#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>

#ifdef __APPLE__
#define GLEW_OK 0
inline int glewInit() { return GLEW_OK; }
inline char* glewGetErrorString(int p) { return "dummy"; }
bool glewExperimental = true;
#else
#include <GL/glew.h>
#include <GL/glu.h>
#endif

#include <SDL.h>
#include <SDL_opengl.h>

#include "util/json/json11.hpp"
#include "util/string.h"
#include "util/import.h"
#include "util/io.h"
#include "util/random.h"
#include "util/csv/CSVRow.h"
#include "engine/core/Context.h"
#include "engine/core/GameLoop.h"
#include "engine/components/Transform.h"
#include "engine/components/Geometry.h"
#include "engine/components/Camera.h"
#include "engine/components/Text.h"
#include "engine/components/Grid.h"
#include "engine/systems/GlRenderSystem.h"
#include "engine/systems/PlayerControlSystem.h"
#include "engine/systems/CameraSystem.h"
#include "engine/systems/TextRenderSystem.h"
#include "engine/systems/GridSystem.h"
#include "engine/systems/StarDataSystem.h"
#include "engine/core/input/SdlInputHandler.h"

std::shared_ptr<Context> ctx = std::make_shared<Context>();

int main (int argc, char** argv) {
  if (argc != 2) {
    std::cout << "Usage: sandbox <config_file>" << std::endl;
    return 1;
  }

  std::string configString = util::loadFile(argv[1]);

  std::string parseErr;
  json11::Json config = json11::Json::parse(configString, parseErr);
  if (parseErr.size() > 0) {
    std::cout << parseErr << std::endl;
    std::exit(EXIT_FAILURE);
  }

  int width = config["width"].int_value();
  int height = config["height"].int_value();

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL could not initialise. SDL_Error: %s\n", SDL_GetError());
    return 1;
  }
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  SDL_Window* window = SDL_CreateWindow(
    "SDL_Window",
    SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED,
    width,
    height,
    SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
  );
  if (window == nullptr) {
    printf("Window could not be created. SDL_Error: %s\n", SDL_GetError());
    return 2;
  }

  SDL_GLContext gContext = SDL_GL_CreateContext(window);
  if (gContext == nullptr) {
    printf("OpenGL context could not be created: %s\n", SDL_GetError());
    return 3;
  }

  //Init GLEW
  glewExperimental = GL_TRUE;
  GLenum glewError = glewInit();
  if (glewError != GLEW_OK) {
    printf("Error initialising GLEW: %s\n", glewGetErrorString(glewError));
  }
  
  ctx->init();
  auto inputHandler = std::make_shared<SdlInputHandler>();
  ctx->registerInputHandler(inputHandler);

  ctx->registerComponent<Transform>();
  ctx->registerComponent<Geometry>();
  ctx->registerComponent<Camera>();
  ctx->registerComponent<Text>();
  ctx->registerComponent<Grid>();

  auto starDataSystem = ctx->registerSystem<StarDataSystem>();
  {
    Signature signature;
    signature.set(ctx->getComponentType<Camera>());
    ctx->setSystemSignature<StarDataSystem>(signature);
    starDataSystem->init(config["starDatabase"].string_value());
  }

  auto cameraSystem = ctx->registerSystem<CameraSystem>();
  {
    Signature signature;
    signature.set(ctx->getComponentType<Camera>());
    ctx->setSystemSignature<CameraSystem>(signature);

    cameraSystem->init();
    inputHandler->addObserver(cameraSystem.get());
  }

  auto renderSystem = ctx->registerSystem<GlRenderSystem>();
  {
    Signature signature;
    signature.set(ctx->getComponentType<Transform>());
    signature.set(ctx->getComponentType<Geometry>());
    ctx->setSystemSignature<GlRenderSystem>(signature);

    renderSystem->init(window, config);
  }

  /*auto playerControlSystem = ctx->registerSystem<PlayerControlSystem>();
  {
    Signature signature;
    signature.set(ctx->getComponentType<Transform>());
    ctx->setSystemSignature<PlayerControlSystem>(signature);

    playerControlSystem->init();
    inputHandler->addObserver(playerControlSystem.get());
  }*/

  auto textRenderSystem = ctx->registerSystem<TextRenderSystem>();
  {
    Signature signature;
    signature.set(ctx->getComponentType<Transform>());
    signature.set(ctx->getComponentType<Text>());
    ctx->setSystemSignature<TextRenderSystem>(signature);

    textRenderSystem->init(width, height, window, config);
    textRenderSystem->loadFont("/home/aludlow/projects/gamedev/sandbox/resources/fonts/OCRAEXT.TTF", "OCRAEXT", 24);
  }

  auto gridSystem = ctx->registerSystem<GridSystem>();
  {
    Signature signature;
    signature.set(ctx->getComponentType<Grid>());
    signature.set(ctx->getComponentType<Transform>());
    ctx->setSystemSignature<GridSystem>(signature);

    gridSystem->init(window, config);
  }

  Geometry geom = util::importShape(config["shapefile"].string_value());

  struct Star {
    std::string sourceId;
    double x, y, z, distFromOrigin;
    std::string name;
  };

  std::ifstream starFile(config["star_data"].string_value());
  util::csv::CSVRow row;
  std::vector<Star> stars;
  // Process header
  starFile >> row;
  while(starFile >> row) {
    stars.push_back(Star { .sourceId = std::string(row[0]), .x = ::atof(row[1].data()), .y = ::atof(row[2].data()), .z = ::atof(row[3].data()), .distFromOrigin = ::atof(row[4].data()), .name = std::string(row[5].data()) });
  }

  // Create star entities
  double shapeScaleX = config["shapeScaleX"].is_null() ? 1.0f : config["shapeScaleX"].number_value();
  double shapeScaleY = config["shapeScaleY"].is_null() ? 1.0f : config["shapeScaleY"].number_value();
  double shapeScaleZ = config["shapeScaleZ"].is_null() ? 1.0f : config["shapeScaleZ"].number_value();
  for (auto i : stars) {
    auto x = i.x;
    auto y = i.y;
    auto z = i.z;

    Entity object = ctx->createEntity();

    ctx->addComponent<Transform>(
      object,
      Transform {
        .position = glm::vec3(x, y, z),
        .rotation = glm::vec3(0.0f, 0.0f, 0.0f),
        .scale = glm::vec3(shapeScaleX, shapeScaleY, shapeScaleZ)
      }
    );
    ctx->addComponent<Geometry>(
      object,
      geom
    );
    if (config["starNames"].bool_value()) {
      ctx->addComponent<Text>(
        object,
        Text {
          .text = i.name,
          .fontsize = 24,
          .colour = glm::vec3(2.0f)
        }
      );
    }
  }

  if (config["grid"].bool_value()) {
    Entity coordGridEntity = ctx->createEntity();
    Grid coordGrid = createGrid(50, 50, 1.0f);
    ctx->addComponent<Grid>(
      coordGridEntity,
      coordGrid
    );

    ctx->addComponent<Transform>(
      coordGridEntity,
      Transform {
        .position = glm::vec3(-25.0f, -25.0f, 0.0f)
      }
    );

    Entity coordGridEntity2 = ctx->createEntity();
    Grid coordGrid2 = createGrid(50, 50, 1.0f);
    ctx->addComponent<Grid>(
      coordGridEntity2,
      coordGrid2
    );

    ctx->addComponent<Transform>(
      coordGridEntity2,
      Transform {
        .position = glm::vec3(-25.0f, -25.0f, -10.0f)
      }
    );
  }

  Entity camera = ctx->createEntity("camera");
  glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 1.0f);
  ctx->addComponent<Camera>(
    camera,
    Camera { 
      .position = cameraPosition,
      .front = glm::vec3(0.0f, 0.0f, -1.0f),
      .up = glm::vec3(0.0f, 1.0f, 0.0f),
      .right = glm::vec3(1.0f, 0.0f, 0.0f),
      .direction = glm::vec3(0.0f, 0.0f, -1.0f),
      .worldUp = glm::vec3(0.0f, 1.0f, 0.0f),
      .fov = 45.0f,
      .speed = config["camera_speed"].number_value(),
      .yaw = -90.0
    }
  );

  auto gameLoop = GameLoop(inputHandler, ctx);
  inputHandler->addObserver(&gameLoop);

  gameLoop.run();

  return 0;
}

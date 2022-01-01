#include <iostream>
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
#include "engine/core/Context.h"
#include "engine/core/GameLoop.h"
#include "engine/components/Transform.h"
#include "engine/components/Geometry.h"
#include "engine/components/Camera.h"
#include "engine/systems/GlRenderSystem.h"
#include "engine/systems/PlayerControlSystem.h"
#include "engine/systems/CameraSystem.h"
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

  auto playerControlSystem = ctx->registerSystem<PlayerControlSystem>();
  {
    Signature signature;
    signature.set(ctx->getComponentType<Transform>());
    ctx->setSystemSignature<PlayerControlSystem>(signature);

    playerControlSystem->init();
    //inputHandler->addObserver(playerControlSystem.get());
  }

  Geometry geom = util::importShape(config["shapefile"].string_value());
  // Create random scene
  for (int i = 0; i < 10000; i++) {
    float x = util::random(-1000.0f, 1000.0f);
    float y = util::random(-1000.0f, 1000.0f);
    float z = util::random(-1000.0f, 1000.0f);

    Entity object = ctx->createEntity();
    ctx->addComponent<Transform>(
      object,
      Transform {
        .position = glm::vec3(x, y, z),
        .rotation = glm::vec3(0.0f, 0.0f, 0.0f),
        .scale = glm::vec3(0.5f, 0.5f, 0.5f)
      }
    );
    ctx->addComponent<Geometry>(
      object,
      geom
    );
  }

  Entity camera = ctx->createEntity("camera");
  ctx->addComponent<Camera>(
    camera,
    Camera { 
      .position = glm::vec3(0.0f, 0.0f, 3.0f),
      .front = glm::vec3(0.0f, 0.0f, -1.0f),
      .up = glm::vec3(0.0f, 1.0f, 0.0f),
      .right = glm::vec3(1.0f, 0.0f, 0.0f),
      .direction = glm::vec3(0.0f, 0.0f, 0.0f),
      .worldUp = glm::vec3(0.0f, 1.0f, 0.0f),
      .speed = config["camera_speed"].number_value(),
    }
  );

  auto gameLoop = GameLoop(inputHandler, ctx);
  inputHandler->addObserver(&gameLoop);

  gameLoop.run();

  return 0;
}

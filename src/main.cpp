#include <iostream>
#include <cstdlib>
#include <string>

#include <SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <GL/glu.h>

#include "util/json/json11.hpp"
#include "util/string.h"
#include "util/import.h"
#include "util/io.h"
#include "engine/core/Context.h"
#include "engine/core/GameLoop.h"
#include "engine/components/Transform.h"
#include "engine/components/Geometry.h"
#include "engine/components/View.h"
//#include "engine/systems/SdlRenderSystem.h"
#include "engine/systems/GlRenderSystem.h"
#include "engine/systems/PlayerControlSystem.h"
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
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
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
  /*
  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (renderer == nullptr) {
    printf("Renderer could not be created. SDL Error: %s\n", SDL_GetError());
    return 3;
  }
  */
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
  ctx->registerComponent<View>();

  auto renderSystem = ctx->registerSystem<GlRenderSystem>();
  {
    Signature signature;
    signature.set(ctx->getComponentType<Transform>());
    signature.set(ctx->getComponentType<Geometry>());
    signature.set(ctx->getComponentType<View>());
    ctx->setSystemSignature<GlRenderSystem>(signature);

    renderSystem->init(window);
  }

/*
  auto renderSystem = ctx->registerSystem<SdlRenderSystem>();
  {
    Signature signature;
    signature.set(ctx->getComponentType<Transform>());
    signature.set(ctx->getComponentType<Geometry>());
    signature.set(ctx->getComponentType<View>());
    ctx->setSystemSignature<SdlRenderSystem>(signature);

    renderSystem->init(renderer);
  }

  auto playerControlSystem = ctx->registerSystem<PlayerControlSystem>();
  {
    Signature signature;
    signature.set(ctx->getComponentType<Transform>());
    ctx->setSystemSignature<PlayerControlSystem>(signature);

    playerControlSystem->init();
    inputHandler->addObserver(playerControlSystem.get());
  }

  Entity player = ctx->createEntity();
  ctx->addComponent<Transform>(
    player,
    Transform {
      .position = glm::vec3(0.0f, 0.0f, 0.0f),
      .rotation = glm::vec3(0.0f, 0.0f, 0.0f),
      .scale = glm::vec3(50.0f, 50.0f, 50.0f)
    }
  );
  ctx->addComponent<Geometry>(
    player,
    util::importShape("/home/aludlow/projects/gamedev/monkey.obj")
    //util::importShape("/home/aludlow/projects/gamedev/cube.obj")
  );
  ctx->addComponent<View>(
    player,
    View { glm::vec3(0.0f, 0.0f, 75.0f) }
  );
  */

  auto gameLoop = GameLoop(inputHandler, ctx);
  inputHandler->addObserver(&gameLoop);

  gameLoop.run();

  return 0;
}

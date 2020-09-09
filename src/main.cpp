#include <iostream>

#include <SDL.h>

#include "engine/core/Context.h"
#include "engine/core/GameLoop.h"
#include "engine/components/Transform.h"
#include "engine/components/Geometry.h"
#include "engine/systems/SdlRenderSystem.h"
#include "engine/systems/PlayerControlSystem.h"
#include "engine/core/input/SdlInputHandler.h"

std::shared_ptr<Context> ctx = std::make_shared<Context>();

int main (int argc, char** argv) {
  if (argc != 3) {
    std::cout << "Usage: sandbox <wdth> <height>" << std::endl;
    return 1;
  }

  int width = std::stoi(argv[1]);
  int height = std::stoi(argv[2]); 

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL could not initialise. SDL_Error: %s\n", SDL_GetError());
    return 1;
  }
  SDL_Window* window = SDL_CreateWindow(
    "SDL_Window",
    SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED,
    width,
    height,
    SDL_WINDOW_SHOWN
  );
  if (window == nullptr) {
    printf("Window could not be created. SDL_Error: %s\n", SDL_GetError());
    return 2;
  }
  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (renderer == nullptr) {
    printf("Renderer could not be created. SDL Error: %s\n", SDL_GetError());
    return 3;
  }

  ctx->init();
  auto inputHandler = std::make_shared<SdlInputHandler>();
  ctx->registerInputHandler(inputHandler);

  ctx->registerComponent<Transform>();
  ctx->registerComponent<Geometry>();

  auto renderSystem = ctx->registerSystem<SdlRenderSystem>();
  {
    Signature signature;
    signature.set(ctx->getComponentType<Transform>());
    signature.set(ctx->getComponentType<Geometry>());
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
      .position = glm::vec3(width/2, height/2, 0.0f),
      .rotation = glm::vec3(0.0f, 0.0f, 0.0f),
      .scale = glm::vec3(20.0f, 20.0f, 1.0f)
    }
  );
  ctx->addComponent<Geometry>(
    player,
    Geometry  {
      .vertices = {
        glm::vec4(-1.0f, -1.0f, 0.0f, 1.0f),
        glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
        glm::vec4(1.0f, -1.0f, 0.0f, 1.0f)
      }
    }
  );

  auto gameLoop = GameLoop(inputHandler, ctx);
  inputHandler->addObserver(&gameLoop);

  gameLoop.run();

  /*
  if (ctx->init(width, height) != 0) {
    std::cout << "Failed to initialise sandbox context." << std::endl;
    return 1;
  }

  std::unique_ptr<InputHandler> inputHandler = std::make_unique<SdlInputHandler>();
  GameLoop gameLoop = GameLoop(inputHandler.get());
  inputHandler->addObserver(&gameLoop);

  gameLoop.run();
  */

  return 0;
}

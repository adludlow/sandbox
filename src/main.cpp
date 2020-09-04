#include <iostream>

#include <SDL.h>

#include "engine/core/Context.h"
#include "engine/components/Transform.h"
#include "engine/components/Geometry.h"
#include "engine/systems/RenderSystem.h"

Context ctx;

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

  ctx.init();

  ctx.registerComponent<Transform>();
  ctx.registerComponent<Geometry>();

  auto renderSystem = ctx.registerSystem<RenderSystem>();
  Signature signature;
  signature.set(ctx.getComponentType<Transform>());
  signature.set(ctx.getComponentType<Geometry>());
  ctx.setSystemSignature<RenderSystem>(signature);

  renderSystem->init(renderer);

  Entity player = ctx.createEntity();
  ctx.addComponent<Transform>(
    player,
    Transform {
      .position = glm::vec3(width/2, height/2, 0.0f),
      .rotation = glm::vec3(0.0f, 0.0f, 0.0f),
      .scale = glm::vec3(1.0f, 1.0f, 1.0f)
    }
  );
  ctx.addComponent<Geometry>(
    player,
    Geometry  {
      .vertices = {
        glm::vec3(-1.0f, -1.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(1.0f, -1.0f, 0.0f)
      }
    }
  );

  /*
  if (ctx.init(width, height) != 0) {
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

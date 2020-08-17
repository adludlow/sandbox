#include "Context.hpp"
#include "Timer.hpp"

int Context::init(int width, int height) {
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
  renderer_ = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (renderer_ == nullptr) {
    printf("Renderer could not be created. SDL Error: %s\n", SDL_GetError());
    return 3;
  }
  return 0;
}

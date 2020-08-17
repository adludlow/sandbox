#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include <SDL.h>

#include "Entity.hpp"

class Context {
  public:
    Context() {};

    const int SCREEN_FPS = 60;
    const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

    int init(int width, int height);
    int run();

  private:
    bool running_;
    Entity* entities_;

    SDL_Renderer* renderer_;
};

#endif

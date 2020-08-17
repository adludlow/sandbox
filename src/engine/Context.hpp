#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include <SDL.h>

#include "Entity.hpp"
#include "input/InputHandler.hpp"

class Context {
  public:
    Context() {};

    int init(int width, int height);

  private:
    Entity* entities_;

    SDL_Renderer* renderer_;
};

#endif

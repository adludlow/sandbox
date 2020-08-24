#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include <SDL.h>
#include <memory>
#include <vector>
#include <unordered_map>

#include "component/component.hpp"
#include "input/InputHandler.hpp"

class Context {
  public:
    Context() {};

    int init(int width, int height);

  private:
    std::vector<int> entities_;
    std::unordered_map<int, std::vector<std::unique_ptr<component::component>>> components_;    

    SDL_Renderer* renderer_;
};

#endif

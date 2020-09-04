#pragma once

#include <SDL.h>

#include "../core/System.h"

class RenderSystem : public System {
  public:
    std::set<Entity> entities() override {
      return entities_;
    }

    void addEntity(Entity entity) override {
      entities_.insert(entity);
    }
    void deleteEntity(Entity entity) override {
      entities_.erase(entity);
    }

    void init(SDL_Renderer* renderer) {
      renderer_ = renderer;
    }

    void update() {

    }

  private:
    std::set<Entity> entities_{};
    SDL_Renderer* renderer_;
};


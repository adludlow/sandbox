#pragma once

#include <SDL.h>
#include <iostream>

#include "../core/Context.h"
#include "../core/System.h"
#include "../components/Transform.h"
#include "../components/Geometry.h"

extern std::shared_ptr<Context> ctx;

class SdlRenderSystem : public System {
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

    void update(float dt) override {
      SDL_SetRenderDrawColor(renderer_, 0, 0, 0, SDL_ALPHA_OPAQUE);
      SDL_RenderClear(renderer_);
      SDL_SetRenderDrawColor(renderer_, 255, 255, 255, SDL_ALPHA_OPAQUE);
      for (auto const& entity : entities_) {
        auto& transform = ctx->getComponent<Transform>(entity);
        auto& geometry = ctx->getComponent<Geometry>(entity);

        // TODO: SDLs coordinate system is inverted. Need to invert objects.
        std::vector<SDL_Point> sdlPoints;
        for (auto i = 0lu; i < geometry.vertices.size(); i++) {
          glm::vec4 pos = geometry.vertices[i] * transform.scale + transform.position;
          sdlPoints.push_back({ static_cast<int>(round(pos.x)), static_cast<int>(round(pos.y)) });
          if (i != 0 && (i+1) % 3 == 0) {
            glm::vec4 initPos = geometry.vertices[i-2] * transform.scale + transform.position;
            sdlPoints.push_back({ static_cast<int>(round(initPos.x)), static_cast<int>(round(initPos.y)) });
            SDL_RenderDrawLines(renderer_, sdlPoints.data(), sdlPoints.size());
            sdlPoints.clear();
          } 
        }
      }
      SDL_RenderPresent(renderer_);
    }

  private:
    std::set<Entity> entities_{};
    SDL_Renderer* renderer_;
};


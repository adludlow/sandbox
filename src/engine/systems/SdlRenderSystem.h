#pragma once

#include <SDL.h>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

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

    glm::vec4 invert(glm::vec4 vert) {
      return glm::vec4(
        vert.x - 2 * vert.x,
        vert.y - 2 * vert.y,
        vert.z,
        vert.w
      );
    }

    glm::vec3 invert(glm::vec3 vert) {
      return glm::vec3(
        vert.x - 2 * vert.x,
        vert.y - 2 * vert.y,
        vert.z
      );
    }

    void update(float dt) override {
      SDL_SetRenderDrawColor(renderer_, 0, 0, 0, SDL_ALPHA_OPAQUE);
      SDL_RenderClear(renderer_);
      SDL_SetRenderDrawColor(renderer_, 255, 255, 255, SDL_ALPHA_OPAQUE);
      for (auto const& entity : entities_) {
        auto& transform = ctx->getComponent<Transform>(entity);
        auto& geometry = ctx->getComponent<Geometry>(entity);

        std::vector<SDL_Point> sdlPoints;
        for (auto i = 0lu; i < geometry.vertices.size(); i++) {
          auto transMat = glm::translate(glm::mat4(1.0f), transform.position);
          auto scaleMat = glm::scale(glm::mat4(1.0f), transform.scale);
          auto rotQuat = glm::quat(glm::vec4(transform.rotation.x, transform.rotation.y, transform.rotation.z, 0.0));
          auto rotMat = glm::toMat4(rotQuat);

          auto proj = glm::perspective(glm::radians(45.0f), 1680.0f/1050.0f, 0.1f, 100.0f);
          auto pos = proj * transMat * rotMat * scaleMat * geometry.vertices[i];
          sdlPoints.push_back({ static_cast<int>(round(pos.x)), static_cast<int>(round(pos.y)) });
          if (i != 0 && (i+1) % 3 == 0) {
            auto initPos = proj * transMat * rotMat * scaleMat * geometry.vertices[i-2];
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


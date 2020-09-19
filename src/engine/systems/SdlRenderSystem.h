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
          glm::mat4 transMat = glm::mat4(1.0);
          // Rotate
          glm::quat rotQuat = glm::quat(glm::vec4(transform.rotation.x, transform.rotation.y, transform.rotation.z, 0.0));
          glm::mat4 rotMat = glm::toMat4(rotQuat);
          // Translate
          transMat = glm::translate(transMat, transform.position);
          // Scale
          transMat = glm::scale(transMat, transform.scale);
          glm::vec4 pos = transMat * rotMat * geometry.vertices[i];
          sdlPoints.push_back({ static_cast<int>(round(pos.x)), static_cast<int>(round(pos.y)) });
          if (i != 0 && (i+1) % 3 == 0) {
            glm::vec4 initPos = transMat * geometry.vertices[i-2];
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


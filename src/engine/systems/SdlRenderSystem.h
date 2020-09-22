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
      int w, h = 0;
      SDL_GetRendererOutputSize(renderer_, &w, &h);
      screenWidth_ = w;
      screenHeight_ = h;
      std::cout << screenWidth_ << " " << screenHeight_ << std::endl;
      screenWidthOffset_ = screenWidth_/2;
      screenHeightOffset_ = screenHeight_/2;
      std::cout << screenWidthOffset_ << " " << screenHeightOffset_ << std::endl;
    }

    SDL_Point toSdlCoords(glm::vec4 pos) {
      return {
        static_cast<int>(round(pos.x + screenWidthOffset_)),
        static_cast<int>(round(-pos.y + screenHeightOffset_))
      };
    }

    void update(float dt) override {
      SDL_SetRenderDrawColor(renderer_, 0, 0, 0, SDL_ALPHA_OPAQUE);
      SDL_RenderClear(renderer_);
      SDL_SetRenderDrawColor(renderer_, 255, 255, 255, SDL_ALPHA_OPAQUE);
      for (auto const& entity : entities_) {
        auto& transform = ctx->getComponent<Transform>(entity);
        auto& geometry = ctx->getComponent<Geometry>(entity);
        auto& view = ctx->getComponent<View>(entity);

        std::vector<SDL_Point> sdlPoints;
        for (auto i = 0lu; i < geometry.vertices.size(); i++) {
          auto transMat = glm::translate(glm::mat4(1.0f), transform.position);
          auto scaleMat = glm::scale(glm::mat4(1.0f), transform.scale);
          auto rotQuat = glm::quat(glm::vec4(transform.rotation.x, transform.rotation.y, transform.rotation.z, 0.0));
          auto rotMat = glm::toMat4(rotQuat);
          auto viewMat = glm::mat4(1.0f);
          viewMat = glm::translate(viewMat, view.position);
          auto proj = glm::perspective(glm::radians(45.0f), 1680.0f/1050.0f, 0.1f, 100.0f);

          auto pos = proj * viewMat * transMat * rotMat * scaleMat * geometry.vertices[i];
          sdlPoints.push_back(toSdlCoords(pos));
          if (i != 0 && (i+1) % 3 == 0) {
            auto initPos = proj * transMat * rotMat * scaleMat * geometry.vertices[i-2];
            sdlPoints.push_back(toSdlCoords(initPos));
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
    int screenWidth_{0};
    int screenHeight_{0};
    float screenWidthOffset_{0.0f};
    float screenHeightOffset_{0.0f};
};


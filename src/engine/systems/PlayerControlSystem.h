#pragma once

#include <memory>
#include <string>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "../core/System.h"
#include "../core/input/InputObserver.h"
#include "../util.h"

extern std::shared_ptr<Context> ctx;

class PlayerControlSystem : public System, public InputObserver {
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

    void init() {
      id_ = util::uuid();
    }

    void update(float dt) override {

    }

    std::string id() override {
      return id_;
    }

    void onNotifyInput(const std::vector<InputEvent>& events) override {
      glm::quat rotQuat = glm::quat(glm::vec4(0.05, 0.0, 0.0, 0.0));
      for (auto event: events) {
        switch (event) {
          case InputEvent::MoveForwards:
            // Move player
            for (auto const& entity : entities_) {
              auto& transform = ctx->getComponent<Transform>(entity);
              auto& geometry = ctx->getComponent<Geometry>(entity);
              //transform.position.y += 1;
              for (auto& v : geometry.vertices) {
                v = v * glm::toMat4(rotQuat);
                std::cout << v.x << " " << v.y << " " << v.z << std::endl;
              }
            }
            break;
          case InputEvent::MoveBackwards:
            // Move player
            for (auto const& entity : entities_) {
              auto& transform = ctx->getComponent<Transform>(entity);
              transform.position.y -= 1;
            }
            break;
          default:
            break;
        }
      }
    }

  private:
    std::set<Entity> entities_{};
    std::string id_;
};

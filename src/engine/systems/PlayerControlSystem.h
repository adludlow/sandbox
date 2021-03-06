#pragma once

#include <memory>
#include <string>

#include "../core/System.h"
#include "../core/input/InputObserver.h"
#include "../../util/uuid.h"

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

    void onNotifyInput(const std::vector<InputEvent>& events, float dt) override {
      for (auto event: events) {
        switch (event.type) {
          case InputEventType::MoveForwards:
            // Move player up
            for (auto const& entity : entities_) {
              auto& transform = ctx->getComponent<Transform>(entity);
              transform.position.y += 0.1;
            }
            break;
          case InputEventType::MoveBackwards:
            // Move player down
            for (auto const& entity : entities_) {
              auto& transform = ctx->getComponent<Transform>(entity);
              transform.position.y -= 0.1;
            }
            break;
          case InputEventType::RotateClockwise:
            // Rotate player clockwise
            for (auto const& entity : entities_) {
              auto& transform = ctx->getComponent<Transform>(entity);
              transform.rotation.y += 0.1;
            }
            break;
          case InputEventType::RotateAntiClockwise:
            // Rotate player anti clockwise
            for (auto const& entity : entities_) {
              auto& transform = ctx->getComponent<Transform>(entity);
              transform.rotation.y -= 0.1;
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

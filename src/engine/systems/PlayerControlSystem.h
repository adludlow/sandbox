#pragma once

#include <memory>
#include <string>

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
      std::cout << "MOVE" << std::endl;
      for (auto event: events) {
        switch (event) {
          case InputEvent::MoveForwards:
            // Move player
            for (auto const& entity : entities_) {
              auto& transform = ctx->getComponent<Transform>(entity);
              transform.position.y += 1;
              std::cout << transform.position.y << std::endl;
            }
            break;
          case InputEvent::MoveBackwards:
            // Move player
            for (auto const& entity : entities_) {
              auto& transform = ctx->getComponent<Transform>(entity);
              transform.position.y -= 1;
              std::cout << transform.position.y << std::endl;
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

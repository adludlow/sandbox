#pragma once

#include <memory>

#include "../core/System.h"
#include "../core/input/InputObserver.h"

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
    }

    void update(float dt) override {

    }

    void onNotifyInput(const std::vector<InputEvent>& events) override {
      for (auto event: events) {
        switch (event) {
          case InputEvent::MoveForwards:
            // Move player
            break;
          default:
            break;
        }
      }
    }

  private:
    std::set<Entity> entities_{};
};

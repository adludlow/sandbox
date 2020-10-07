#pragma once

#include <memory>

#include "../core/System.h"
#include "../components/Camera.h"

extern std::shared_ptr<Context> ctx;

class CameraSystem : public System {
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

    void update(float df) override {
      for (auto entity: entities_) {
        auto& camera = ctx->getComponent<Camera>(entity);

      }
    }

  private:
    std::set<Entity> entities_;
};

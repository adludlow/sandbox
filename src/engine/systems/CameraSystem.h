#pragma once

#include <memory>
#include <string>

#include <iostream>

#include "../core/System.h"
#include "../core/input/InputObserver.h"
#include "../components/Camera.h"
#include "../../util/uuid.h"

extern std::shared_ptr<Context> ctx;

class CameraSystem : public System, public InputObserver {
  public:
    std::set<Entity> entities() override {
      return entities_;
    }

    std::string id() override {
      return id_;
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

    void update(float df) override {
      for (auto entity: entities_) {
        //auto& camera = ctx->getComponent<Camera>(entity);

      }
    }

    void onNotifyInput(const std::vector<InputEvent>& events, float dt) {
      for (auto entity: entities_) {
        auto& camera = ctx->getComponent<Camera>(entity);
        float velocity = camera.speed * dt;
        for (InputEvent event: events) {
          switch (event.type) {
            case InputEventType::MoveForwards:
              camera.position += camera.front * velocity;
              break;
            case InputEventType::MoveBackwards:
              camera.position -= camera.front * velocity;
              break;
            case InputEventType::MoveRight:
              camera.position += camera.right * velocity;
              break;
            case InputEventType::MoveLeft:
              camera.position -= camera.right * velocity;
              break;
            case InputEventType::MouseMove:
              float xOffset = event.mouseMove.xrel * sensitivity_;
              float yOffset = event.mouseMove.yrel * sensitivity_;
              camera.yaw += xOffset;
              camera.pitch -= yOffset;

              // calculate the new Front vector
              glm::vec3 front;
              front.x = cos(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
              front.y = sin(glm::radians(camera.pitch));
              front.z = sin(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
              camera.front = glm::normalize(front);
              // also re-calculate the Right and Up vector
              camera.right = glm::normalize(glm::cross(camera.front, camera.worldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
              camera.up = glm::normalize(glm::cross(camera.right, camera.front));

              break;
          }
        }
      }
    }

  private:
    std::set<Entity> entities_;
    std::string id_;
    float sensitivity_ { 0.07f };
};

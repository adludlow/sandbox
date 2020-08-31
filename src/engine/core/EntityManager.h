#pragma once

#include <array>
#include <queue>
#include <cassert>

#include "types.h"

class EntityManager {
  public:
    EntityManager() {
      for (Entity entity = 0; entity < MAX_ENTITIES; ++entity) {
        availableEntities_.push(entity);
      }
    }

    Entity createEntity() {
      assert(activeEntityCount_ < MAX_ENTITIES && "Max entities reached.");

      Entity id = availableEntities_.front();
      availableEntities_.pop();
      ++activeEntityCount_;

      return id;
    }

    void destroyEntity(Entity entity) {
      assert(entity < MAX_ENTITIES && "Entitiy out of range.");

      signatures_[entity].reset();
      availableEntities_.push(entity);
      --activeEntityCount_;
    }

    void setSignature(Entity entity, Signature signature) {
      assert(entity < MAX_ENTITIES && "Entity out of range.");

      signatures_[entity] = signature;
    }

    Signature getSignature(Entity entity) {
      assert(entity < MAX_ENTITIES && "Entity out of range.");

      return signatures_[entity];
    }

    uint32_t activeEntityCount() {
      return activeEntityCount_;
    }

  private:
    std::queue<Entity> availableEntities_{};
    std::array<Signature, MAX_ENTITIES> signatures_{};
    uint32_t activeEntityCount_{};
};

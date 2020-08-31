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

    Entity CreateEntity() {
      assert(livingEntityCount_ < MAX_ENTITIES && "Max entities reached.");

      Entity id = availableEntities_.front();
      availableEntities_.pop();
      ++livingEntityCount_;

      return id;
    }

    void DestroyEntity(Entity entity) {
      assert(entity < MAX_ENTITIES && "Entitiy out of range.");

      signatures_[entity].reset();
      availableEntities_.push(entity);
      --livingEntityCount_;
    }

    void SetSignature(Entity entity, Signature signature) {
      assert(entity < MAX_ENTITIES && "Entity out of range.");

      signatures_[entity] = signature;
    }

    Signature GetSignature(Entity entity) {
      assert(entity < MAX_ENTITIES && "Entity out of range.");

      return signatures_[entity];
    }

  private:
    std::queue<Entity> availableEntities_{};
    std::array<Signature, MAX_ENTITIES> signatures_{};
    uint32_t livingEntityCount_{};
};

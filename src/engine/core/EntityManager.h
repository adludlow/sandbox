#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <array>
#include <queue>
#include <cassert>
#include <string>
#include <unordered_map>

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

    Entity createEntity(const std::string& name) {
      Entity id = createEntity();
      namedEntities_.insert({ name, id });
      return id;
    }

    void deleteEntity(Entity entity) {
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

    Entity getNamedEntity(const std::string& name) {
      return namedEntities_[name];
    }

    uint32_t activeEntityCount() {
      return activeEntityCount_;
    }

  private:
    std::queue<Entity> availableEntities_{};
    std::unordered_map<Entity, Signature> signatures_{};
    uint32_t activeEntityCount_{};
    std::unordered_map<std::string, Entity> namedEntities_{};
};

#endif // ENTITY_MANAGER_H

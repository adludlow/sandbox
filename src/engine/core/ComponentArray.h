#pragma once

#include <array>
#include <unordered_map>
#include <assert.h>

#include "types.h"

class IComponentArray {
  public:
    virtual ~IComponentArray() {};
    virtual void entityDeleted(Entity entity) = 0;
};

template<typename T>
class ComponentArray : public IComponentArray {
  public:
    void addComponent(Entity entity, T component) {
      assert(entityToIndexMap_.find(entity) == entityToIndexMap_.end() && "Component added to same entity more than once.");

      // Add component at end.
      size_t newIndex = size_;
      entityToIndexMap_[entity] = newIndex;
      indexToEntityMap_[newIndex] = entity;
      componentArray_[newIndex] = component;
      ++size_;
    }

    void removeComponent(Entity entity);
    T& getComponent(Entity entity);

    void entityDeleted(Entity entity) override {}

    size_t size() {
      return size_;
    }

  private:
    std::array<T, MAX_ENTITIES> componentArray_{};
    std::unordered_map<Entity, size_t> entityToIndexMap_{};
    std::unordered_map<size_t, Entity> indexToEntityMap_{};
    size_t size_ = 0;
};

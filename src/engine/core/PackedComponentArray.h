#pragma once

#include "ComponentArray.h"

template<typename T>
class PackedComponentArray : public ComponentArray {
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

    void removeComponent(Entity entity) {
      assert(entityToIndexMap_.find(entity) != entityToIndexMap_.end() && "Attempted to remove component that does not exist.");

      // Copy last element to deleted elements place in array. This maintains array density.
      size_t indexOfRemovedElement = entityToIndexMap_[entity];
      size_t indexOfLastElement = size_ - 1;
      componentArray_[indexOfRemovedElement] = componentArray_[indexOfLastElement];

      // Update maps to reflect changes above.
      Entity entityOfLastElement = indexToEntityMap_[indexOfLastElement];
      entityToIndexMap_[entityOfLastElement] = indexOfRemovedElement;
      indexToEntityMap_[indexOfRemovedElement] = entityOfLastElement;

      entityToIndexMap_.erase(entity);
      indexToEntityMap_.erase(indexOfLastElement);

      --size_;
    }

    T& getComponent(Entity entity) {
      assert(entityToIndexMap_.find(entity) != entityToIndexMap_.end() && "Attempted to retrieve component that does not exist.");

      return componentArray_[entityToIndexMap_[entity]];
    }

    void entityDeleted(Entity entity) override {
      if (entityToIndexMap_.find(entity) != entityToIndexMap_.end()) {
        removeComponent(entity);
      }
    }

    size_t size() {
      return size_;
    }

  private:
    //std::array<T, MAX_ENTITIES> componentArray_{};
    std::unordered_map<size_t, T> componentArray_{};
    std::unordered_map<Entity, size_t> entityToIndexMap_{};
    std::unordered_map<size_t, Entity> indexToEntityMap_{};
    size_t size_ = 0;
};
#pragma once

#include <unordered_map>
#include <memory>
#include <typeinfo>
#include <assert.h>

#include "types.h"
#include "PackedComponentArray.h"

class ComponentManager {
  public:
    template<typename T>
    void registerComponent() {
      const char* typeName = typeid(T).name();

      assert(componentTypes_.find(typeName) == componentTypes_.end() && "Attempted to register component more than once.");

      // Add new component to maps
      componentTypes_.insert({ typeName, nextComponentType_ });
      componentArrays_.insert({ typeName, std::make_shared<PackedComponentArray<T>>() });

      ++nextComponentType_;
    }

    template<typename T>
    ComponentType getComponentType() {
      const char* typeName = typeid(T).name();

      assert(componentTypes_.find(typeName) != componentTypes_.end() && "Requested unregistered component type.");

      return componentTypes_[typeName];
    }

    template<typename T>
    void addComponent(Entity entity, T component) {
      getComponentArray<T>()->addComponent(entity, component);
    }

    template<typename T>
    void removeComponent(Entity entity) {
      getComponentArray<T>()->removeComponent(entity);
    }

    template<typename T>
    T& getComponent(Entity entity) {
      return getComponentArray<T>()->getComponent(entity);
    }

    void entityDeleted(Entity entity) {
      for (auto const& pair : componentArrays_) {
        auto const& component = pair.second;
        component->entityDeleted(entity);
      }
    }

  private:
    std::unordered_map<const char*, ComponentType> componentTypes_ {};
    std::unordered_map<const char*, std::shared_ptr<ComponentArray>> componentArrays_ {};
    ComponentType nextComponentType_ {};

    template<typename T>
    std::shared_ptr<PackedComponentArray<T>> getComponentArray() {
      const char* typeName = typeid(T).name();

      assert(componentTypes_.find(typeName) != componentTypes_.end() && "Requested unregistered component type.");

      return std::static_pointer_cast<PackedComponentArray<T>>(componentArrays_[typeName]);
    }
};

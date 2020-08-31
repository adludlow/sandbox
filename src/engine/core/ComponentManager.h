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
      GetComponentArray<T>()->addComponent(entity, component);
    }

    template<typename T>
    void removeComponent(Entity entity) {
      GetComponentArray<T>()->removeComponent(entity);
    }

    template<typename T>
    T& getComponent(Entity entity) {
      return GetComponentArray<T>()->getComponent(entity);
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
    std::shared_ptr<ComponentArray> GetComponentArray() {
      const char* typeName = typeid(T).name();

      assert(componentTypes_.find(typeName) != componentTypes_.end() && "Requested unregistered component type.");

      return std::static_pointer_cast<ComponentArray>(componentArrays_[typeName]);
    }
};

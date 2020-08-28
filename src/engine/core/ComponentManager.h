#pragma once

#include <unordered_map>
#include <memory>
#include <typeinfo>
#include <assert.h>

#include "types.h"
#include "ComponentArray.h"

class ComponentManager {
  public:
    template<typename T>
    void registerComponent() {
      const char* typeName = typeid(T).name();

      assert(componentTypes_.find(typeName) == componentTypes_.end() && "Attempted to register component more than once.");

      // Add new component to maps
      componentTypes_.insert({ typeName, nextComponentType_ });
      componentArrays_.insert({ typeName, std::make_unique<ComponentArray<T>>() });

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
    std::unordered_map<const char*, std::unique_ptr<IComponentArray>> componentArrays_ {};
    ComponentType nextComponentType_ {};

    template<typename T>
    std::unique_ptr<ComponentArray<T>> GetComponentArray() {
      const char* typeName = typeid(T).name();

      assert(componentTypes_.find(typeName) != componentTypes_.end() && "Requested unregistered component type.");

      return std::static_pointer_cast<ComponentArray<T>>(componentArrays_[typeName]);
    }
};

#pragma once

#include <unordered_map>
#include <memory>

#include "types.h"

class ComponentManager {
  public:
    template<typename T>
    void registerComponent();

    template<typename T>
    void getComponentType();

    template<typename T>
    void addComponent(Entity entity, T component);

    template<typename T>
    void removeComponent(Entity entity);

    template<typename T>
    T& getComponent(Entity entity);

    void entityDeleted(Entity entity);

  private:
    std::unordered_map<const char*, std::unique_ptr<IComponentArray>> componentArrays {};
};
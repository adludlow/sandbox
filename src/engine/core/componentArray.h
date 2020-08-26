#pragma once

#include <array>

#include "types.h"

class IComponentArray {
  public:
    virtual ~IComponentArray() {};
    virtual void entityDeleted(Entity entity) = 0;
};

template<typename T>
class ComponentArray : public IComponentArray {
  public:
    void addComponent(Entity entity, T component);
    void removeComponent(Entity entity);
    T& getComponent(Entity entity);
    void entityDeleted(Entity entity) override;

  private:
    std::array<T, MAX_ENTITIES> componentArray_;
    stze_t size_
};

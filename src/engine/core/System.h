#pragma once

#include <set>

#include "types.h"

class System {
  public:
    virtual ~System() {};
    virtual std::set<Entity> entities() = 0;
    virtual void addEntity(Entity entity) = 0;
    virtual void deleteEntity(Entity entity) = 0;
};

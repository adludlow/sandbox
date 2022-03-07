#ifndef SYSTEM_H
#define SYSTEM_H

#include <set>

#include "types.h"

class System {
  public:
    virtual ~System() {};
    virtual std::set<Entity> entities() = 0;
    virtual void addEntity(Entity entity) = 0;
    virtual void deleteEntity(Entity entity) = 0;
    virtual void update(float dt) = 0;
};

#endif // SYSTEM_H

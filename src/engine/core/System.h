#pragma once

#include <set>

#include "types.h"

class System {
  public:
    virtual ~System() {};
    virtual std::set<Entity> entities() = 0;
};

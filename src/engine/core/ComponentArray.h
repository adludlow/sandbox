#pragma once

#include <array>
#include <unordered_map>
#include <assert.h>

#include "types.h"

class ComponentArray {
  public:
    virtual ~ComponentArray() {};
    
    virtual void entityDeleted(Entity entity) = 0;
};

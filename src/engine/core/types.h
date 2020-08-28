#pragma once

#include <cstdint>

// Entity
using Entity = std::uint32_t;
const Entity MAX_ENTITIES = 10000;

//ComponentType
using ComponentType = std::uint16_t;
const ComponentType MAX_COMPONENTS = 64;

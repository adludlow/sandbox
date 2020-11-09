#pragma once

#include <cstdint>
#include <bitset>

// Entity
using Entity = std::uint32_t;
const Entity MAX_ENTITIES = 100000;

//ComponentType
using ComponentType = std::uint16_t;
const ComponentType MAX_COMPONENTS = 64;

//Signature
using Signature = std::bitset<MAX_COMPONENTS>;

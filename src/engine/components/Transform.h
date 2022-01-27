#pragma once

#include <glm/vec3.hpp>

using namespace glm;

struct Transform {
  vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
  vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
  vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
};

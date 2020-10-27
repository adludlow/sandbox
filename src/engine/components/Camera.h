#pragma once

#include <glm/vec3.hpp>

using namespace glm;

struct Camera {
  vec3 position;
  vec3 front;
  vec3 up;
  vec3 right;
  vec3 direction;
  float speed { 0.01f };
};

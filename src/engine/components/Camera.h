#pragma once

#include <glm/vec3.hpp>

using namespace glm;

struct Camera {
  vec3 position;
  vec3 front;
  vec3 up;
  vec3 right;
  vec3 direction;
  vec3 worldUp;
  double speed { 0.01 };
  double yaw { 0.0 };
  double pitch { 0.0 };
};

#pragma once

#include <glm/vec4.hpp>
#include <vector>
#include <iostream>

using namespace glm;

struct Geometry {
  std::vector<vec4> vertices;

  void print() {
    for (auto v : vertices) {
      std::cout << v.x << "," << v.y << "," << v.z << "," << v.w << std::endl;
    }
  }
};

#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <glm/vec4.hpp>
#include <vector>
#include <iostream>

using namespace glm;

struct Geometry {
  std::vector<vec4> vertices;
  std::vector<uint> indices;

  void print() {
    for (auto v : vertices) {
      std::cout << v.x << "," << v.y << "," << v.z << "," << v.w << std::endl;
    }
  }
};

#endif //GEOMETRY_H

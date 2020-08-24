#ifndef SPATIAL_HPP
#define SPATIAL_HPP

#include <vector>

#include "../geom/Polygon.hpp"

namespace component {
  struct spatial : public component {
    Vector vector;
    std::vector<Polygon> polygons;
  };
}

#endif

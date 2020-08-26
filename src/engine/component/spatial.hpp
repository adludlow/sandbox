#ifndef SPATIAL_HPP
#define SPATIAL_HPP

#include <vector>

#include "../geom/Polygon.hpp"

namespace component {
  class Spatial {
    public:
      Spatial(Vector location) : locationVector_(location) {};
      void addPolygon(const Polygon& p);
      void addPolygons(const std::vector<Polygon>& p);
    private:
      Vector locationVector_;
      std::vector<Polygon> polygons_;
  };
}

#endif

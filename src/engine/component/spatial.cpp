#include "spatial.hpp"

using namespace component;
void Spatial::addPolygon(const Polygon& polygon) {
  polygons_.push_back(polygon);
}

void Spatial::addPolygons(const std::vector<Polygon>& polygons) {
  polygons_.insert(std::end(polygons_), std::begin(polygons), std::end(polygons));
}

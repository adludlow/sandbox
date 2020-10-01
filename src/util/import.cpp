#include <fstream>

#include "import.h"
#include "string.h"

Geometry util::importShape(std::string path) {
  std::ifstream ifs(path);
  std::string line;
  Geometry geom{};
  while (std::getline(ifs, line)) {
    if (line.rfind("v ", 0) == 0) {
      auto coords = util::split(line);
      coords.erase(coords.begin());
      geom.vertices.push_back(
        glm::vec4(
          std::stod(coords[0]),
          std::stod(coords[1]),
          std::stod(coords[2]),
          1.0f
        )
      );
    } else if (line.rfind("f ", 0) == 0) {
      auto face_verts = util::split(line);
      face_verts.erase(face_verts.begin());
      for (auto idx_trip : face_verts) {
        auto idxs = util::split(idx_trip, '/');
        geom.indices.push_back(std::stod(idxs[0])-1);
      }
    }
  }
  return geom;
}

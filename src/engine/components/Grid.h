#ifndef GRID_H
#define GRID_H

#include <glm/glm.hpp>

struct Grid {
  unsigned int numColumns = 1;
  unsigned int numRows = 1;
  float cellSize = 1.0f;
  std::vector<glm::vec4> vertices;
  std::vector<glm::uint> indices;
};

Grid createGrid(unsigned int numColumns, unsigned int numRows, float cellSize) {
  Grid grid;

  grid.numColumns = numColumns;
  grid.numRows = numRows;
  grid.cellSize = cellSize;
  for (unsigned int y = 0; y <= numRows; y++) {
    for (unsigned int x = 0; x <= numColumns; x++) {
      grid.vertices.push_back(glm::vec4(x*cellSize, y*cellSize, 0.0f, 1.0f));
    }
  }

  // Indices
  // x-axis
  for (unsigned int i = 0; i <= numColumns; i++) {
    int offset = i*(numColumns+1);
    grid.indices.push_back(offset);
    grid.indices.push_back(offset+numColumns);
  }
  // y-axis
  for (unsigned int i = 0; i <= numRows; i++) {
    int offset = (numColumns+1)*numRows;
    grid.indices.push_back(i);
    grid.indices.push_back(i+offset);
  }

  return grid;
}

#endif //GRID_H

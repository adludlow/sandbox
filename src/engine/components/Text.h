#ifndef TEXT_H
#define TEXT_H

#include <string>
#include <glm/vec3.hpp>

struct Text {
  std::string text;
  unsigned int fontsize;
  glm::vec3 colour;
};

#endif //TEXT_H

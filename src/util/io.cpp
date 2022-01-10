#include <fstream>
#include <iostream>

#include "io.h"

std::string util::loadFile(const std::string& path) {
  std::ifstream ifs(path);
  std::string fileContent(
    (std::istreambuf_iterator<char>(ifs)),
    (std::istreambuf_iterator<char>())
  );
  ifs.close();
  return fileContent;
}

std::ostream& operator<<(std::ostream& os, const glm::vec3& vec)
{
    os << vec.x << ',' << vec.y << ',' << vec.z;
    return os;
}

std::ostream& operator<<(std::ostream& os, const glm::vec4& vec)
{
    os << vec.x << ',' << vec.y << ',' << vec.z << ',' << vec.w;
    return os;
}
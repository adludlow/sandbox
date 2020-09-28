#include <fstream>

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

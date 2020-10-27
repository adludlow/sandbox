#pragma once

#include <string>
#include <iostream>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace util {
  std::string loadFile(const std::string& path);
}

std::ostream& operator<<(std::ostream& os, const glm::vec3& vec);

std::ostream& operator<<(std::ostream& os, const glm::vec4& vec);

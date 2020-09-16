#pragma once

#include <string>
#include <sstream>

namespace util {
  std::vector<std::string> split(const std::string& str, char delim = ' ') {
    std::vector<std::string> cont;
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, delim)) {
      cont.push_back(token);
    }
    return cont;
  }
}

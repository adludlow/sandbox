#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <memory>
#include <string>
#include <map>

#include "Shader.h"
#include "../../util/io.h"

class ResourceManager {
  public:
    static std::map<std::string, Shader> shaders;

    static Shader loadShader(const std::string& vertexFile, const std::string& fragmentFile, const std::string& name);
    static Shader getShader(const std::string& name);

  private:
    ResourceManager() {}

    static Shader loadShaderFromFile(const std::string& vertexFile, const std::string& fragmentFile);
};

#endif //RESOURCE_MANAGER_H
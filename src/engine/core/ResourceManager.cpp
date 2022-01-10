#include <iostream>

#include "ResourceManager.h"

std::map<std::string, Shader> ResourceManager::shaders;


Shader ResourceManager::loadShader(const std::string& vertexFile, const std::string& fragmentFile, const std::string& name) {
  shaders[name] = loadShaderFromFile(vertexFile, fragmentFile);
  return shaders[name];
}

Shader ResourceManager::getShader(const std::string& name) {
  return shaders[name];
}

Shader ResourceManager::loadShaderFromFile(const std::string& vertexFile, const std::string& fragmentFile) {
  std::string vertexString = util::loadFile(vertexFile);
  std::string fragmentString = util::loadFile(fragmentFile);
  Shader shader;
  shader.compile(vertexString, fragmentString);
  return shader;
}

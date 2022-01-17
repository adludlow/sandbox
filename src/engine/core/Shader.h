#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#include <GL/glu.h>
#endif
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
  public:
    unsigned int id;

    Shader() {}

    Shader& use() {
      glUseProgram(this->id);
      return *this;
    }

    void compile(const std::string& vertexString, const std::string& fragmentString) {
      unsigned int sVertex, sFragment;

      // vertex
      sVertex = glCreateShader(GL_VERTEX_SHADER);
      const GLchar* vertexSource = vertexString.c_str();
      glShaderSource(sVertex, 1, &vertexSource, NULL);
      glCompileShader(sVertex);
      checkCompileErrors(sVertex, "VERTEX");

      // fragment
      sFragment = glCreateShader(GL_FRAGMENT_SHADER);
      const GLchar* fragmentSource = fragmentString.c_str();
      glShaderSource(sFragment, 1, &fragmentSource, NULL);
      glCompileShader(sFragment);
      checkCompileErrors(sFragment, "FRAGMENT");

      this->id = glCreateProgram();
      glAttachShader(this->id, sVertex);
      glAttachShader(this->id, sFragment);
      glLinkProgram(this->id);
      checkCompileErrors(this->id, "PROGRAM");

      glDeleteShader(sVertex);
      glDeleteShader(sFragment);

    }

    void setInteger(const std::string& name, int value, bool useShader = false) {
      if (useShader) {
        this->use();
      }
      glUniform1i(glGetUniformLocation(this->id, name.c_str()), value);
    }

    void setMatrix4(const std::string& name, const glm::mat4& matrix, bool useShader = false) {
      if (useShader) {
        this->use();
      }
      glUniformMatrix4fv(glGetUniformLocation(this->id, name.c_str()), 1, false, glm::value_ptr(matrix));
    }

    void setVector3f(const std::string& name, const glm::vec3& value, bool useShader = false) {
      if (useShader) {
        this->use();
      }
      glUniform3f(glGetUniformLocation(this->id, name.c_str()), value.x, value.y, value.z);
    }

  private:
    void checkCompileErrors(unsigned int object, std::string type) {
      int success;
      char infoLog[1024];
      if (type != "PROGRAM")
      {
          glGetShaderiv(object, GL_COMPILE_STATUS, &success);
          if (!success)
          {
              glGetShaderInfoLog(object, 1024, NULL, infoLog);
              std::cout << "| ERROR::SHADER: Compile-time error: Type: " << type << "\n"
                  << infoLog << "\n -- --------------------------------------------------- -- "
                  << std::endl;
          }
      }
      else
      {
          glGetProgramiv(object, GL_LINK_STATUS, &success);
          if (!success)
          {
              glGetProgramInfoLog(object, 1024, NULL, infoLog);
              std::cout << "| ERROR::Shader: Link-time error: Type: " << type << "\n"
                  << infoLog << "\n -- --------------------------------------------------- -- "
                  << std::endl;
          }
      }
    } 
};

#endif //SHADER_H

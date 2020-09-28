#pragma once

#include <iostream>

#include <SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <GL/glu.h>

#include "../core/System.h"
#include "../../util/io.h"

extern std::shared_ptr<Context> ctx;

class GlRenderSystem : public System {
  public:
    std::set<Entity> entities() override {
      return entities_;
    }

    void addEntity(Entity entity) override {
      entities_.insert(entity);
    }

    void deleteEntity(Entity entity) override {
      entities_.erase(entity);
    }
    
    void init(SDL_Window* window) {
      window_ = window;

      float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f
      };

      // Create program to attach shaders to.
      glProgramId_ = glCreateProgram();

      // Create vertex shader
      GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
      // Load vertex shader
      std::string vertexString = util::loadFile("/home/aludlow/projects/gamedev/sandbox/src/shaders/vertex.glsl");
      const GLchar* vertexShaderSource = vertexString.c_str();
      glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
      glCompileShader(vertexShader);
      int success;
      char infoLog[512];
      glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
      if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "Vertex shader compilation failed: " << infoLog << std::endl;
      }

      // Create fragment shader
      GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
      std::string fragmentString = util::loadFile("/home/aludlow/projects/gamedev/sandbox/src/shaders/fragment.glsl");
      const GLchar* fragmentShaderSource = fragmentString.c_str();
      glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
      glCompileShader(fragmentShader);
      glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
      if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "Fragment shader compilation failed: " << infoLog << std::endl;
      }

      glAttachShader(glProgramId_, vertexShader);
      glAttachShader(glProgramId_, fragmentShader);
      glLinkProgram(glProgramId_);
      glGetProgramiv(glProgramId_, GL_LINK_STATUS, &success);
      if (!success) {
        glGetProgramInfoLog(glProgramId_, 512, NULL, infoLog);
        std::cout << "Shader linking failed: " << infoLog << std::endl;
      }

      glDeleteShader(vertexShader);
      glDeleteShader(fragmentShader);

      glGenVertexArrays(1, &glVao_);
      // Generate vertex buffer object
      glGenBuffers(1, &glVbo_);

      // Bind Vertex Array Object
      glBindVertexArray(glVao_);

      // Bind buffer to GL_ARRAY_BUFFER for a vertex buffer
      glBindBuffer(GL_ARRAY_BUFFER, glVbo_);
      // Copy vertices into buffer
      glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
      glEnableVertexAttribArray(0);

      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glBindVertexArray(0);
    }

    void update(float dt) override {
      glClearColor( 0.2f, 0.2f, 0.2f, 1.0f );
      glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      glUseProgram(glProgramId_);
      glBindVertexArray(glVao_);

      glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
      glDrawArrays(GL_TRIANGLES, 0, 3);
      SDL_GL_SwapWindow(window_);

      glBindVertexArray(0);
    }
  
  private:
    std::set<Entity> entities_{};

    SDL_Window* window_{};
    GLuint glProgramId_{};
    GLuint glVao_{};
    GLuint glVbo_{};
};

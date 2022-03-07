#ifndef GL_RENDER_SYSTEM_H
#define GL_RENDER_SYSTEM_H

#include <iostream>
#include <functional>

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#include <GL/glu.h>
#endif

#include <SDL.h>
#include <SDL_opengl.h>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
//#include <glm/gtc/type_ptr.hpp>

#include "../core/System.h"
#include "../core/Shader.h"
#include "../core/ResourceManager.h"
#include "../../util/io.h"
#include "../../util/json/json11.hpp"

extern std::shared_ptr<Context> ctx;

static const uint MAX_BUFF_VERTS = 10000;
static const uint VERT_SIZE = sizeof(glm::vec4);
static const uint MAX_VERT_BUFF_SIZE = MAX_BUFF_VERTS * VERT_SIZE;
static const uint INDEX_SIZE = sizeof(uint);
static const uint MAX_INDEX_BUFF_SIZE = MAX_BUFF_VERTS * INDEX_SIZE;

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
    
    void init(SDL_Window* window, json11::Json config) {
      ctx->addPreUpdateHandler(std::bind(&GlRenderSystem::preUpdate, this));
      ctx->addPostUpdateHandler(std::bind(&GlRenderSystem::postUpdate, this));
      window_ = window;
      int width, height = 0;
      SDL_GetWindowSize(window_, &width, &height);
      this->ratio_ = (float)width/(float)height;

      std::string shaderDir = config["shader_dir"].string_value();
      this->shader_ = ResourceManager::loadShader(shaderDir + "/vertex.glsl", shaderDir + "/fragment.glsl", "geomShader");

      glGenVertexArrays(1, &glVao_);
      // Generate vertex and element buffer object
      glGenBuffers(1, &glVbo_);
      glGenBuffers(1, &glEbo_);

      // Bind Vertex Array Object
      glBindVertexArray(glVao_);

      // Bind buffer to GL_ARRAY_BUFFER for a vertex buffer
      glBindBuffer(GL_ARRAY_BUFFER, glVbo_);
      // Copy vertices into buffer
      glBufferData(GL_ARRAY_BUFFER, MAX_VERT_BUFF_SIZE, NULL, GL_STATIC_DRAW);

      // Element buffer
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glEbo_);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, MAX_INDEX_BUFF_SIZE, NULL, GL_STATIC_DRAW);

      glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
      glEnableVertexAttribArray(0);

      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
      glBindVertexArray(0);
    }

    void update(float dt) override {
      this->shader_.use();
      glBindVertexArray(glVao_);

      for (auto entity : entities_) {
        auto& geometry = ctx->getComponent<Geometry>(entity);
        auto& transform = ctx->getComponent<Transform>(entity);

        Entity cameraEntity = ctx->getNamedEntity("camera");
        auto& camera = ctx->getComponent<Camera>(cameraEntity);

        auto transMat = glm::translate(glm::mat4(1.0f), transform.position);
        auto scaleMat = glm::scale(glm::mat4(1.0f), transform.scale);
        auto rotQuat = glm::quat(glm::vec4(transform.rotation.x, transform.rotation.y, transform.rotation.z, 0.0));
        auto rotMat = glm::toMat4(rotQuat);
        auto proj = glm::perspective(glm::radians((float)camera.fov), ratio_, 0.1f, 900.0f);

        glm::mat4 view = glm::lookAt(camera.position, camera.position + camera.front, camera.up);
        glm::mat4 trans = proj * view * transMat * rotMat * scaleMat;
        unsigned int transformLoc = glGetUniformLocation(this->shader_.id, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
        // Vertices
        glBindBuffer(GL_ARRAY_BUFFER, glVbo_);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec4) * geometry.vertices.size(), geometry.vertices.data());
        // Indices
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glEbo_);
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(uint) * geometry.indices.size(), geometry.indices.data());

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawElements(GL_TRIANGLES, geometry.indices.size(), GL_UNSIGNED_INT, 0);
      }

      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glBindVertexArray(0);
    }

    void preUpdate() {
      glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
      glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void postUpdate() {
      SDL_GL_SwapWindow(window_);
    }
  
  private:
    std::set<Entity> entities_{};

    SDL_Window* window_{};
    Shader shader_;
    GLuint glVao_{};
    GLuint glVbo_{};
    GLuint glEbo_{};
    float ratio_{0.0f};
};

#endif // GL_RENDER_SYSTEM_H

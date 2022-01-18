#ifndef TEST_RENDER_SYSTEM_H
#define TEXT_RENDER_SYSTEM_H

#include <memory>
#include <string>
#include <map>
#include <iostream>

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#include <GL/glu.h>
#endif

#include <SDL.h>
#include <SDL_opengl.h>

#include <glm/gtc/matrix_transform.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "../core/System.h"
#include "../core/Shader.h"
#include "../../util/json/json11.hpp"

extern std::shared_ptr<Context> ctx;

struct Character {
  unsigned int textureId;
  glm::ivec2 size;
  glm::ivec2 bearing;
  unsigned int advance;
};

class TextRenderSystem : public System {
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

    void loadFont(const std::string& fontLoc, const std::string& fontName, const unsigned int fontSize) {
      std::map<char, Character> characters {};
      FT_Library ft;
      if (FT_Init_FreeType(&ft)) {
        std::cerr << "ERROR::FREETYPE: Failed to initialise Freetype Library" << std::endl;
      }

      FT_Face face;
      if (FT_New_Face(ft, fontLoc.c_str(), 0, &face)) {
        std::cerr << "ERROR::FREETYPE: Failed to load font: " << fontLoc << std::endl;
      }

      // Set font size
      FT_Set_Pixel_Sizes(face, 0, fontSize);
      // Disable byte-alignment restriction
      glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
      for (GLubyte c = 0; c < 128; c++) {
        // Load character glyph
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
          std::cerr << "ERROR::FREETYPE: Failed to load glyph" << std::endl;
          continue;
        }
        // Generate texture
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
          GL_TEXTURE_2D,
          0,
          GL_RED,
          face->glyph->bitmap.width,
          face->glyph->bitmap.rows,
          0,
          GL_RED,
          GL_UNSIGNED_BYTE,
          face->glyph->bitmap.buffer
        );
        // Set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Store character for later use
        Character character = {
          texture,
          glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
          glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
          face->glyph->advance.x
        };
        characters.insert(std::pair<char, Character>(c, character));
      }
      fonts_.insert(std::make_pair(std::make_pair(fontName, fontSize), characters));

      // Cleanup
      glBindTexture(GL_TEXTURE_2D, 0);
      FT_Done_Face(face);
      FT_Done_FreeType(ft);
    }

    void init(unsigned int width, unsigned int height, SDL_Window* window, json11::Json config) {
      this->window_ = window;
      //this->projMat_ = glm::ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height));
      this->projMat_ = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);

      std::string shaderDir = config["shader_dir"].string_value();
      this->shader_ = ResourceManager::loadShader(shaderDir + "/text_2d_vert.glsl", shaderDir + "/text_2d_frag.glsl", "textShader");
      ratio_ = width/height;
      //this->shader_.setMatrix4("projection", glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f), true);
      this->shader_.setInteger("text", 0);

      glGenVertexArrays(1, &this->glVao_);
      glGenBuffers(1, &this->glVbo_);
      glBindVertexArray(this->glVao_);
      glBindBuffer(GL_ARRAY_BUFFER, this->glVbo_);
      glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 6, NULL, GL_DYNAMIC_DRAW);
      glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(4 * sizeof(float)));
      glEnableVertexAttribArray(1);
      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glBindVertexArray(0);

      // Load fonts
      loadFont("C:/Users/aludlow/source/gamedev/sandbox/resources/fonts/OCRAEXT.TTF", "OCRAEXT", 16);
    }

    void update(float dt) override {
      this->shader_.use();
      glActiveTexture(GL_TEXTURE0);
      glBindVertexArray(this->glVao_);
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

      for (auto entity : this->entities_) {
        auto& text = ctx->getComponent<Text>(entity);
        this->shader_.setVector3f("textColour", text.colour);
        auto& transform = ctx->getComponent<Transform>(entity);

        Entity cameraEntity = ctx->getNamedEntity("camera");
        auto& camera = ctx->getComponent<Camera>(cameraEntity);

        auto transMat = glm::translate(glm::mat4(1.0f), transform.position);
        auto proj = glm::perspective(glm::radians(45.0f), ratio_, 0.1f, 500.0f);
        glm::mat4 view = glm::lookAt(camera.position, camera.position + camera.front, camera.up);
        glm::vec4 clipSpacePos = proj * (view * glm::vec4(transform.position, 1.0f));
        glm::vec3 ndcSpacePos = glm::vec3(clipSpacePos)/clipSpacePos.w;
        glm::vec2 windowSpacePos = ((glm::vec2(ndcSpacePos) + 1.0f)/2.0f) * glm::vec2(800, 600);
        float x = windowSpacePos.x;
        float y = windowSpacePos.y;
        this->shader_.setMatrix4("transform", this->projMat_);
        std::map<char, Character> glyphs = this->fonts_[std::make_pair("OCRAEXT", 16)];
        std::string::const_iterator c;
        for (c = text.text.begin(); c != text.text.end(); c++) {
          Character ch = glyphs[*c];

          float xpos = x + ch.bearing.x;
          float ypos = y + (glyphs['H'].bearing.y - ch.bearing.y);
          float w = ch.size.x;
          float h = ch.size.y;
          float vertices[6][6] = {
            { xpos,     ypos + h, 0.0f, 1.0f, 0.0f, 0.0f },
            { xpos + w, ypos,     0.0f, 1.0f, 1.0f, 1.0f },
            { xpos,     ypos,     0.0f, 1.0f, 0.0f, 1.0f },
            
            { xpos,     ypos + h, 0.0f, 1.0f, 0.0f, 0.0f },
            { xpos + w, ypos + h, 0.0f, 1.0f, 1.0f, 0.0f },
            { xpos + w, ypos,     0.0f, 1.0f, 1.0f, 1.0f }
          };

          glBindTexture(GL_TEXTURE_2D, ch.textureId);
          // Update contents of VBO memory
          glBindBuffer(GL_ARRAY_BUFFER, this->glVbo_);
          glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
          glBindBuffer(GL_ARRAY_BUFFER, 0);
          // Render quad
          glDrawArrays(GL_TRIANGLES, 0, 6);
          x += (ch.advance >> 6);
        }
      }
      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glBindVertexArray(0);
      glBindTexture(GL_TEXTURE_2D, 0);
    }

  private:
    SDL_Window* window_{};
    std::set<Entity> entities_{};
    std::map<std::pair<std::string, unsigned int>, std::map<char, Character>> fonts_{};
    Shader shader_;
    GLuint glVao_{};
    GLuint glVbo_{};
    glm::mat4 projMat_{};
    float ratio_{0.0f};
};

#endif //TEXT_RENDER_SYSTEM_H

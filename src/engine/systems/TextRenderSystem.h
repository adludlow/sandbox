#ifndef TEST_RENDER_SYSTEM_H
#define TEXT_RENDER_SYSTEM_H

#include <memory>
#include <string>
#include <map>
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "../core/System.h"

extern std::shared_ptr<Context> ctx;

struct Character {
  unsigned int textureId;
  glm::ivec2 size;
  glm::ivec2 bearing;
  unsigned int advance;
};

class TestRenderSystem : public System {
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

    void loadFont(const std::string& font, const unsigned int fontSize) {
      std::map<char, Character> characters {};
      FT_Library ft;
      if (FT_Init_FreeType(&ft)) {
        std::cerr << "ERROR::FREETYPE: Failed to initialise Freetype Library" << std::endl;
      }

      FT_Face face;
      if (FT_New_Face(ft, font.c_str(), 0, &face)) {
        std::cerr << "ERROR::FREETYPE: Failed to load font: " << font << std::endl;
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
      glBindTexture(GL_TEXTURE_2D, 0);
      // Cleanup
      FT_Done_Face(face);
      FT_Done_FreeType(ft);
    }

    void init() {
      // Load fonts
    }

    void update(float dt) override {

    }

  private:
    std::set<Entity> entities_{};
};

#endif //TEXT_RENDER_SYSTEM_H

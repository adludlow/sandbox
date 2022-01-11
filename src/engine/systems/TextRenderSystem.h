#ifndef TEST_RENDER_SYSTEM_H
#define TEXT_RENDER_SYSTEM_H

#include <memory>
#include <string>
#include <map>

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

#include <iostream>

#include "../../../test/catch.hpp"

#include "../System.h"
#include "../SystemManager.h"

class DummySystem : public System {
  public:
    std::set<Entity> entities() override {
      return entities_;
    };

    void addEntity(Entity entity) override {
      entities_.insert(entity);
    }

    void deleteEntity(Entity entity) override {
      entities_.erase(entity);
    }

  private:
    std::set<Entity> entities_{};

};

SCENARIO("Systems are registered with the SystemManager") {
  GIVEN("A SystemManager") {
    SystemManager sysMan;

    WHEN("A system is registered") {
      sysMan.registerSystem<DummySystem>();
    }
  }
}
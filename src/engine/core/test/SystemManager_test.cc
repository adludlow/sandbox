#include <iostream>

#include "../../../test/catch.hpp"

#include "../System.h"
#include "../SystemManager.h"

class DummySystem : public System {
  std::set<Entity> entities() {
    return std::set<Entity>();
  };
};

SCENARIO("Systems are registered with the SystemManager") {
  GIVEN("A SystemManager") {
    SystemManager sysMan;

    WHEN("A system is registered") {
      sysMan.registerSystem<DummySystem>();
    }
  }
}
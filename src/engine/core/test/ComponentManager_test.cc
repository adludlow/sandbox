#include <iostream>

#include "../../../test/catch.hpp"

#include "../ComponentManager.h"
#include "DummyComponent.h"

SCENARIO("Component are registered with a ComponentManager", "[ComponentManager]") {
  GIVEN("A ComponentManager") {
    ComponentManager componentManager;

    WHEN("A component is Registered") {
      componentManager.registerComponent<DummyComponent>();
      
      THEN("The component type can be retrieved from the ComponentManager") {
        ComponentType ct = componentManager.getComponentType<DummyComponent>();
        REQUIRE(1 == 1);
      }
    }
  }
}
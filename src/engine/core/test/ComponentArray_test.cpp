#define CATCH_CONFIG_MAIN

#include "../../../test/catch.hpp"

#include "../engine/core/PackedComponentArray.h"
#include "DummyComponent.h"

SCENARIO("Components are added and removed from a ComponentArray", "[ComponentArray]") {
  GIVEN("An empty ComponentArray") {
    PackedComponentArray<DummyComponent> componentArray;
    DummyComponent dummy;
    WHEN("A component is added") {
      componentArray.addComponent(1, dummy);
      
      THEN("The size of the array is incrementd") {
        REQUIRE(componentArray.size() == 1);
      }
    }

    WHEN("A component is added then deleted") {
      componentArray.addComponent(1, dummy);
      componentArray.removeComponent(1);

      THEN("The size of the array is reduced by 1") {
        REQUIRE(componentArray.size() == 0);
      }
    }
  }
}

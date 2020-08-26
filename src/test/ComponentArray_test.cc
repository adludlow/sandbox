#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "../engine/core/ComponentArray.h"

struct DummyComponent {
  int id;
  int data;
};

SCENARIO("Components are added to a ComponentArray", "[ComponentArray]") {
  GIVEN("A ComponentArray") {
    ComponentArray<DummyComponent> componentArray;

    WHEN("A component is added") {
      DummyComponent dummy;
      componentArray.addComponent(1, dummy);
      
      THEN("The size of the array is incrementd") {
        REQUIRE(componentArray.size() == 1);
      }
    }
  }
}

#include "../../../test/catch.hpp"

#include "../EntityManager.h"

SCENARIO("Create and Destroy Entities", "[EntityManager]") {
  GIVEN("An EntityManager") {
    EntityManager entityManager;

    WHEN("A component is created") {
      Entity entity = entityManager.createEntity(); 

      THEN("Active entity count increases") {
        REQUIRE(entityManager.activeEntityCount() == 1);
      }
    }
  }
}

cmake_minimum_required(VERSION 2.8)
project(sandbox)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${sandbox_SOURCE_DIR}/cmake")

set(BIN_DIR ${sandbox}/bin)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -std=c++17")
if(APPLE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -framework SDL2")
  if(FRAMEWORKS_LOC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -F${FRAMEWORKS_LOC}")
  endif()
endif()

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG} -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE} -O2")

if(NOT SDL_HEADERS)
  set(SDL_HEADERS "/usr/include/SDL2")
endif()

if(NOT BOOST_HEADERS)
  set(BOOST_HEADERS "/usr/include/boost")
endif()

if(NOT GLM_HEADERS)
  set(GLM_HEADERS)
endif()

include_directories(${SDL_HEADERS} ${BOOST_HEADERS} ${GLM_HEADERS} ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(
  sandbox
  src/main.cpp
  src/engine/core/input/SdlInputHandler.cpp
  src/engine/util.cpp
  src/engine/Timer.cpp
  src/engine/core/GameLoop.cpp
)

if(NOT APPLE)
  set(SDL_LIBS /usr/lib/libSDL2.so /usr/lib/libSDL2_image.so)
endif()
target_link_libraries(sandbox ${SDL_LIBS})

install(TARGETS sandbox RUNTIME DESTINATION ${BIN_DIR})

# Testing
add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE src/test)
add_executable(
  engineTest
  src/engine/core/test/ComponentArray_test.cc
  src/engine/core/test/ComponentManager_test.cc
  src/engine/core/test/EntityManager_test.cc
  src/engine/core/test/SystemManager_test.cc
)
target_link_libraries(engineTest PUBLIC Catch)

enable_testing()
add_test(NAME EngineTests COMMAND engineTest)

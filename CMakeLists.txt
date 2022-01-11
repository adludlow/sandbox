cmake_minimum_required(VERSION 3.0)
project(sandbox)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${sandbox_SOURCE_DIR}/cmake")

set(BIN_DIR ${sandbox}/bin)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -std=c++20")
if(APPLE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__gl_h_")
  if(FRAMEWORKS_LOC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -F${FRAMEWORKS_LOC}")
  endif()
  set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -framework SDL2 -framework OpenGL")
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

include_directories(${SDL_HEADERS} ${BOOST_HEADERS} ${GLM_HEADERS} ${OPENGL_HEADERS} ${FREETYPE_HEADERS} ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(
  sandbox
  src/util/json/json11.cpp
  src/main.cpp
  src/engine/core/input/SdlInputHandler.cpp
  src/engine/Timer.cpp
  src/engine/core/GameLoop.cpp
  src/engine/core/ResourceManager.cpp
  src/util/import.cpp
  src/util/random.cpp
  src/util/uuid.cpp
  src/util/string.cpp
  src/util/io.cpp
)

if(UNIX)
  set(SDL_LIB /usr/lib/libSDL2.so)
  set(SDLMAIN_LIB /usr/lib/libSDL2_image.so)
  set(GLEW_LIB /usr/lib/libGLEW.so)
  set(GL_LIB /usr/lib/libGL.so)
elseif(WIN32)
  set(GL_LIB C:\\Windows\\System32\\opengl32.dll)
  set(BCRYPT_LIB C:\\Windows\\System32\\bcrypt.dll)
endif()
target_link_libraries(sandbox ${SDL_LIB} ${SDLMAIN_LIB} ${GLEW_LIB} ${BCRYPT_LIB} ${GL_LIB})

install(TARGETS sandbox RUNTIME DESTINATION ${BIN_DIR})

# Testing
add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE src/test)
add_executable(
  engineTest
  src/engine/core/test/ComponentArray_test.cpp
  src/engine/core/test/ComponentManager_test.cpp
  src/engine/core/test/EntityManager_test.cpp
  src/engine/core/test/SystemManager_test.cpp
)
target_link_libraries(engineTest PUBLIC Catch)

enable_testing()
add_test(NAME EngineTests COMMAND engineTest)

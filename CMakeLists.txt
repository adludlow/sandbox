cmake_minimum_required(VERSION 3.0)
project(sandbox)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${sandbox_SOURCE_DIR}/cmake")

set(BIN_DIR ${sandbox}/bin)
if(WIN32)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest /Y-")
elseif(APPLE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__gl_h_")
  if(FRAMEWORKS_LOC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -F${FRAMEWORKS_LOC}")
  endif()
  set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -framework SDL2 -framework OpenGL")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -std=c++20")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG} -g")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE} -O2")
endif()


if(NOT SDL_HEADERS)
  set(SDL_HEADERS "/usr/include/SDL2")
endif()

if(NOT BOOST_HEADERS)
  set(BOOST_HEADERS "/usr/include/boost")
endif()

if(NOT FREETYPE_HEADERS)
  set(FREETYPE_HEADERS "/usr/include/freetype2")
endif()

if(NOT GLM_HEADERS)
  set(GLM_HEADERS)
endif()

include_directories(${SDL_HEADERS} ${BOOST_HEADERS} ${GLM_HEADERS} ${OPENGL_HEADERS} ${FREETYPE_HEADERS} ${FREETYPE2_HEADERS} ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(
  sandbox
  src/util/json/json11.cpp
  src/main.cpp
  src/engine/core/input/SdlInputHandler.cpp
  src/engine/Timer.cpp
  src/engine/core/GameLoop.cpp
  src/engine/core/ResourceManager.cpp
  src/engine/systems/StarDataSystem.cpp
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
  set(FREETYPE_LIB /usr/lib/libfreetype.so)
  set(SQLITE_LIB /usr/lib/libsqlite3.so)
elseif(WIN32)
  set(GL_LIB "C:/Program Files (x86)/Windows Kits/10/Lib/10.0.22000.0/um/x64/OpenGL32.Lib")
  set(BCRYPT_LIB "C:/Program Files (x86)/Windows Kits/10/Lib/10.0.22000.0/um/x64/bcrypt.lib")
  set(SDL_LIB "C:/DeveloperTools/SDL2-2.0.18/lib/x64/SDL2.lib")
  set(SDLMAIN_LIB "C:/DeveloperTools/SDL2-2.0.18/lib/x64/SDL2main.lib")
  set(FREETYPE_LIB "C:/DeveloperTools/freetype-windows-binaries-2.11.1/release dll/win64/freetype.lib")
  set(GLEW_LIB "C:/DeveloperTools/glew-2.1.0/lib/Release/x64/glew32.lib")
endif()
target_link_libraries(sandbox ${SDL_LIB} ${SDLMAIN_LIB} ${GLEW_LIB} ${BCRYPT_LIB} ${GL_LIB} ${FREETYPE_LIB} ${SQLITE_LIB})

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

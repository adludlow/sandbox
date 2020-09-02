#include <iostream>

#include "engine/core/Context.h"
#include "engine/components/Transform.h"

Context ctx;

int main (int argc, char** argv) {
  if (argc != 3) {
    std::cout << "Usage: sandbox <wdth> <height>" << std::endl;
    return 1;
  }

  int width = std::stoi(argv[1]);
  int height = std::stoi(argv[2]); 

  ctx.init();

  ctx.registerComponent<Transform>();

  /*
  if (ctx.init(width, height) != 0) {
    std::cout << "Failed to initialise sandbox context." << std::endl;
    return 1;
  }

  std::unique_ptr<InputHandler> inputHandler = std::make_unique<SdlInputHandler>();
  GameLoop gameLoop = GameLoop(inputHandler.get());
  inputHandler->addObserver(&gameLoop);

  gameLoop.run();
  */

  return 0;
}

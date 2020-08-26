#include <iostream>

#include "engine/Context.hpp"
#include "engine/GameLoop.hpp"
#include "engine/input/SdlInputHandler.hpp"
#include "engine/component/spatial.hpp"

int main (int argc, char** argv) {
  if (argc != 3) {
    std::cout << "Usage: sandbox <wdth> <height>" << std::endl;
    return 1;
  }

  int width = std::stoi(argv[1]);
  int height = std::stoi(argv[2]); 

  Context ctx = Context();
  if (ctx.init(width, height) != 0) {
    std::cout << "Failed to initialise sandbox context." << std::endl;
    return 1;
  }

  std::unique_ptr<InputHandler> inputHandler = std::make_unique<SdlInputHandler>();
  GameLoop gameLoop = GameLoop(inputHandler.get());
  inputHandler->addObserver(&gameLoop);

  // Initial shape
  Vector center = Vector(width/2, height/2, 0);
  std::vector<Vector> vertices = {
    Vector(center.x()-50, center.y()+50),
    Vector(center.x(), center.y()-50),
    Vector(center.x()+50, center.y()+25),
    Vector(center.x()-50, center.y()+50)
  };
  Polygon trianglePoly(vertices);
  component::Spatial mainFeature(center);
  mainFeature.addPolygon(trianglePoly);

  gameLoop.run();

  return 0;
}

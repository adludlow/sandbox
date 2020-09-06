#ifndef GAMELOOP_HPP
#define GAMELOOP_HPP

#include <vector>
#include <string>

#include "input/InputHandler.h"
#include "Context.h"

class GameLoop: public InputObserver {
  public:
    GameLoop(std::shared_ptr<InputHandler> inputHandler, std::shared_ptr<Context> ctx);

    const int SCREEN_FPS = 60;
    const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

    int run();
    void onNotifyInput(const std::vector<InputEvent>& events);
    std::string id();

  private:
    bool running_;
    std::string id_;
    std::shared_ptr<InputHandler> inputHandler_;
    std::shared_ptr<Context> gameContext_;
};

#endif

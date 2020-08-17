#ifndef GAMELOOP_HPP
#define GAMELOOP_HPP

#include <vector>
#include <string>

#include "input/InputHandler.hpp"

class GameLoop: public InputObserver {
  public:
    GameLoop(InputHandler* inputHandler);

    const int SCREEN_FPS = 60;
    const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

    int run();
    void onNotifyInput(const std::vector<InputEvent>& events);
    std::string id();

  private:
    bool running_;
    std::string id_;
    InputHandler* inputHandler_;
};

#endif

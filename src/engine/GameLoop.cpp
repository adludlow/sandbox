#include <SDL.h>

#include "GameLoop.hpp"
#include "Timer.hpp"
#include "util.hpp"

GameLoop::GameLoop(InputHandler* inputHandler) {
  inputHandler_ = inputHandler;
  id_ = util::uuid();
}

void GameLoop::onNotifyInput(const std::vector<InputEvent>& events) {
  for (auto event: events) {
    switch (event) {
      case InputEvent::QuitToDesktop:
        running_ = false;
        break;
      default:
        break;
    }
  }
}

std::string GameLoop::id() {
  return id_;
}

int GameLoop::run() {
  running_ = true;

  Timer fpsTimer;
  Timer capTimer;
  int countedFrames = 0;
  fpsTimer.start();
  while (running_) {
    capTimer.start();
    float avgFps = countedFrames / (fpsTimer.getTicks()/1000.f);

    // RENDER
    
    inputHandler_->handleInput();

    countedFrames++;
    int frameTicks = capTimer.getTicks();
    if (frameTicks < SCREEN_TICKS_PER_FRAME) {
      SDL_Delay(SCREEN_TICKS_PER_FRAME - frameTicks);
    }
  }

  return 0;
}
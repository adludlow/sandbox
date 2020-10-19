#include <SDL.h>
#include <iostream>

#include "GameLoop.h"
#include "../Timer.h"
#include "../../util/uuid.h"

GameLoop::GameLoop(
  std::shared_ptr<InputHandler> inputHandler,
  std::shared_ptr<Context> ctx
) {
  inputHandler_ = inputHandler;
  gameContext_ = ctx;
  id_ = util::uuid();
}

void GameLoop::onNotifyInput(const std::vector<InputEvent>& events, float dt) {
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
    
    inputHandler_->handleInput(capTimer.getTicks());

    gameContext_->update(capTimer.getTicks());
    countedFrames++;
    int frameTicks = capTimer.getTicks();
    if (frameTicks < SCREEN_TICKS_PER_FRAME) {
      SDL_Delay(SCREEN_TICKS_PER_FRAME - frameTicks);
    }
  }

  return 0;
}
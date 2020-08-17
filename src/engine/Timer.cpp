#include <SDL.h>

#include "Timer.hpp"

Timer::Timer() {
  startTicks_ = 0;
}

void Timer::start() {
  startTicks_ = SDL_GetTicks();
  started_ = true;
}

void Timer::stop() {
  started_ = false;
  startTicks_ = 0;
}

unsigned int Timer::getTicks() {
  unsigned int time = 0;

  if (started_) {
    time = SDL_GetTicks() - startTicks_;
  }
  return time;
}

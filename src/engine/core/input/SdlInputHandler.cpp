#include <iostream>
#include <map>
#include <SDL.h>

#include "SdlInputHandler.h"
#include "InputEvent.h"

void SdlInputHandler::addObserver(InputObserver* observer) {
  if (observer != nullptr) {
    observers[observer->id()] = observer;
  }
}

void SdlInputHandler::removeObserver(InputObserver* observer) {
  if (observer != nullptr) {
    observers.erase(observer->id());
  }
}

void SdlInputHandler::handleInput(float dt) {
  SDL_Event e;
  std::vector<InputEvent> events;
  const Uint8 *keystate = SDL_GetKeyboardState(NULL);
  if (keystate[SDL_SCANCODE_LEFT]) {
    events.push_back(InputEvent { .type = InputEventType::MoveLeft });
  }
  if (keystate[SDL_SCANCODE_RIGHT]) {
    events.push_back(InputEvent { .type = InputEventType::MoveRight });
  }
  if (keystate[SDL_SCANCODE_UP]) {
    events.push_back(InputEvent { .type = InputEventType::MoveForwards });
  }
  if (keystate[SDL_SCANCODE_DOWN]) {
    events.push_back(InputEvent { .type = InputEventType::MoveBackwards });
  }
  if (keystate[SDL_SCANCODE_SPACE]) {
    events.push_back(InputEvent { .type = InputEventType::Shoot });
  }
  while (SDL_PollEvent(&e) != 0) {
    switch (e.type) {
      case SDL_QUIT:
        events.push_back(InputEvent { .type = InputEventType::QuitToDesktop });
        break;
      case SDL_MOUSEMOTION:
        events.push_back(InputEvent { 
          .type = InputEventType::MouseMove,
          .mouseMove = {
            .x = e.motion.x,
            .y = e.motion.y,
            .xrel = e.motion.xrel,
            .yrel = e.motion.yrel
          }
        });
        break;
    }
  }

  for (auto observer: observers) {
    observer.second->onNotifyInput(events, dt);
  }
}

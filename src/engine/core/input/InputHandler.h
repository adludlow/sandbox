#pragma once

#include "InputObserver.h"

class InputHandler {
  public:
    virtual ~InputHandler() {}

    virtual void handleInput(float dt) = 0;

    virtual void addObserver(InputObserver* observer) = 0;
    virtual void removeObserver(InputObserver* observer) = 0;
};

#pragma once

#include <vector>
#include <string>

#include "InputEvent.h"

class InputObserver {
  public:
    virtual ~InputObserver() {}

    virtual void onNotifyInput(const std::vector<InputEvent>& events, float dt) = 0;
    virtual std::string id() = 0;
};

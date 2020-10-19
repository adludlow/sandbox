#pragma once

#include <memory>
#include <vector>
#include <map>

#include "InputHandler.h"

class SdlInputHandler: public InputHandler {
  public:
    SdlInputHandler() {};

    void handleInput(float dt) override;

    virtual void addObserver(InputObserver* observer) override;
    virtual void removeObserver(InputObserver* observer) override;

  private:
    std::map<std::string, InputObserver*> observers;
};

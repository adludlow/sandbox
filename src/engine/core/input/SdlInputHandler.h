#pragma once

#include <memory>
#include <vector>
#include <map>

#include "InputHandler.h"

class SdlInputHandler: public InputHandler {
  public:
    SdlInputHandler() {};

    void handleInput();

    virtual void addObserver(InputObserver* observer);
    virtual void removeObserver(InputObserver* observer);

  private:
    std::map<std::string, InputObserver*> observers;
};

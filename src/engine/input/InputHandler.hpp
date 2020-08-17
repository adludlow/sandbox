#ifndef INPUTHANDLER_HPP
#define INPUTHANDLER_HPP

#include "InputObserver.hpp"

class InputHandler {
  public:
    virtual ~InputHandler() {}

    virtual void handleInput() = 0;

    virtual void addObserver(InputObserver* observer) = 0;
    virtual void removeObserver(InputObserver* observer) = 0;
};

#endif

#ifndef SDLINPUTHANDLER_HPP
#define SDLINPUTHANDLER_HPP

#include <memory>
#include <vector>
#include <map>

#include "InputHandler.hpp"

class SdlInputHandler: public InputHandler {
  public:
    SdlInputHandler() {};

    void handleInput();

    virtual void addObserver(InputObserver* observer);
    virtual void removeObserver(InputObserver* observer);

  private:
    std::map<std::string, InputObserver*> observers;
};

#endif

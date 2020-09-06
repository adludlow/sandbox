#pragma once

class Timer {
  public:
    Timer();

    void start();
    void stop();

    unsigned int getTicks();

  private:
    unsigned int startTicks_;
    bool started_;
};

#ifndef TIMER_HPP
#define TIMER_HPP

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

#endif

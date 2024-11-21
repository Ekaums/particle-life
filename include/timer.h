#pragma once

#include <SDL.h>

class Timer{

  private:
    uint32_t ticks;
    bool started;

  public:
    Timer(): ticks(0), started(false) {}

    // Get real-life time that has passed since last call
    uint32_t getTicks();
};
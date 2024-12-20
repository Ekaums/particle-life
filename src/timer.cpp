#include <SDL.h>
#include "../include/timer.h"

uint32_t Timer::getTicks(){

 if(!started){
    started = true;
    ticks = SDL_GetTicks();
    return 0;
  }

  uint32_t new_ticks = SDL_GetTicks() - ticks;
  ticks += new_ticks;
  return new_ticks;
}

void Timer::unpause(){
  ticks = SDL_GetTicks();
}
#pragma once

#include <SDL.h>
#include "timer.hpp"
#include "screen.hpp"


enum Direction{
  Left,
  Right,
  Up,
  Down
};

class SimState{

  private:
    // why can i init like this again??????
    bool mouse_pressed{false};
    bool quit{false};
    bool paused{false};
    int current_time{0};
    int last_spawn_time{0};
    const int spawn_interval{25};
    float zoom_level{1};
    SDL_Rect src_rect{(WORLD_W - SCREEN_W) / 2, (WORLD_H - SCREEN_H) / 2, SCREEN_W, SCREEN_H};  // Portion of the world to zoom into 
    Timer timer{};

    void requestQuit() { quit = true;}
    void togglePause();
    void handleZoom(float zoom);

    void panScreen(Direction dir);

    void spawnParticle();

  public:
    // Getters
    bool isPaused() const { return paused;}
    bool shouldQuit() const { return quit;}
    const SDL_Rect& getSrcRect() const { return src_rect;}
    float getTicks() { return timer.getTicks();}

    void handleEvents();
};
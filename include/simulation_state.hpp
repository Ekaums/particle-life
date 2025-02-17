#pragma once

#include <SDL.h>
#include "timer.hpp"
#include "screen.hpp"
#include "particle.hpp"


// Direction that user presses for panning screen
enum Direction{
  Left,
  Right,
  Up,
  Down
};

// Handles all user input and simulation state
class SimState{

  private:
    bool mouse_pressed{false}; // Is mouse pressed?
    bool quit{false}; // Has user request quit?
    bool paused{false}; // Is sim paused?

    // Used for spawning particles
    int current_time{0};
    int last_spawn_time{0};
    const int spawn_interval{25};
    Colour spawn_colour{Colour::Red};

    float zoom_level{1}; // Scale of zoom
    SDL_Rect src_rect{(WORLD_W - SCREEN_W) / 2, (WORLD_H - SCREEN_H) / 2, SCREEN_W, SCREEN_H};  // Portion of the world to zoom into 
    Timer timer{};

    void togglePause();
    void handleZoom(float zoom);

    void panScreen(Direction dir);

    void spawnParticle();

    void setParticleColour(Colour col){ spawn_colour = col; };

  public:
    // Getters
    bool isPaused() const { return paused;}
    bool shouldQuit() const { return quit;}
    const SDL_Rect& getSrcRect() const { return src_rect;}
    float getTicks() { return timer.getTicks();}

    // Main API: Handles all user input events
    void handleEvents(); 
};
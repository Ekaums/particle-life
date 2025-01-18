#include <iostream>
#include <vector>
#include <SDL.h>
#include <SDL_rect.h>
#include "../include/screen.h"
#include "../include/particle.h"
#include "../include/timer.h"

std::vector<Particle> g_particles;

// Keeps track of simulation state
struct SimState{
  bool quit{false};
  bool paused{false};
  bool pressed{false};
  const int spawnInterval{50};
  Timer timer{};
};

void handleEvent(SDL_Event event, SimState &state);

int main(int argc, char** argv){

  // Setup SDL window and renderer 
  SDL_Window *window;
  SDL_Renderer *render;
  init(window, render);

  SimState state;
  SDL_Event event;

  int num_particles = std::stoi(argv[1]);

  for(int i = 0; i < num_particles; i++){
    if(i%3==0)
    g_particles.push_back(Particle(R));
    else if (i%3 == 1)
    g_particles.push_back(Particle(G));
    else
    g_particles.push_back(Particle(B));
  }

  while(state.quit == false){
    
    while(SDL_PollEvent(&event) != 0){ // Handle all user input (keyboard clicks, mouse presses)
      handleEvent(event, state);
    }

    /* Game loop */
    if(state.paused)
      // don't render
      continue;

    // Clean screen
    SDL_SetRenderDrawColor(render, 222, 216, 228, 255); 
    SDL_RenderClear(render);

    float time = state.timer.getTicks() / 1000.0f; // Get time elapsed -- for time-based movement

    for(Particle& p : g_particles){ // Update all particles
      p.Move(time); 
      p.Draw(render);
    }
    SDL_RenderPresent(render); // Display
  }

  SDL_DestroyRenderer(render);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

void handleEvent(SDL_Event event, SimState &state){ // TODO: lil messy
  static int lastSpawnTime{0};

  if(event.type == SDL_QUIT){ // User request quit
    state.quit = true;
  }
  else if(event.type == SDL_KEYDOWN){ // Key was pressed
    if(event.key.keysym.sym == SDLK_SPACE){
      state.paused = !state.paused; // Pressing space will pause/unpause
      if(state.paused == false){
        state.timer.unpause(); // If timer gets unpaused, update timer
      }
    }
  }
  else if(event.type == SDL_MOUSEBUTTONDOWN && state.paused == false){ // Mouse was pressed and not currently paused
      int x, y;
      SDL_GetMouseState(&x, &y);
      if(x < 0 || x > SCREEN_W || y < 0 || y > SCREEN_H) return;
      Vector pos{static_cast<float>(x), static_cast<float>(y)};
      g_particles.push_back(Particle(G, pos));
      state.pressed = true;
  }
  else if (event.type == SDL_MOUSEBUTTONUP){ // Mouse released
    state.pressed = false;
  }
  else if (event.type == SDL_MOUSEMOTION){ // Mouse is moving
    if(state.pressed){
      Uint32 currentTime = SDL_GetTicks();
      if(currentTime - lastSpawnTime >= state.spawnInterval){ // Spawn particles at specified rate
        int x, y;
        SDL_GetMouseState(&x, &y);
        if(x < 0 || x > SCREEN_W || y < 0 || y > SCREEN_H) return;
        Vector pos{static_cast<float>(x), static_cast<float>(y)};
        g_particles.push_back(Particle(static_cast<Colour>(x%3), pos));
        lastSpawnTime = currentTime;
      }
    }
  }
}

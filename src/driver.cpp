#include <iostream>
#include <vector>
#include <SDL.h>
#include <SDL_rect.h>
#include "../include/screen.h"
#include "../include/particle.h"
#include "../include/timer.h"


void handleEvent(SDL_Event event);

// TODO: idk ab globals
std::vector<Particle> g_particles;

// Screen
static SDL_Renderer *render; 

// Timer for sim pause/play
static Timer timer{};

// User quit status
static bool quit{false}; 

// User pause status
static bool paused{false};

int main(int argc, char** argv){

  // Setup SDL window and renderer 
  SDL_Window *window;
  init(window, render);

  SDL_Event event;

  int num_particles = std::stoi(argv[1]);

  // Equal amount of each colour
  for(int i = 0; i < num_particles; i++){ // TODO: scuffed
    g_particles.push_back(Particle(static_cast<Colour>(i%3)));
  }

  /* Game loop */
  while(quit == false){

    while(SDL_PollEvent(&event) != 0){ // Handle all user input (keyboard clicks, mouse actions)
      handleEvent(event);
    }
    
    // If paused, don't render
    if(paused) 
    continue;

    // Clean screen to prepare for next frame
    SDL_SetRenderDrawColor(render, 222, 216, 228, 255); 
    SDL_RenderClear(render);
    float time = timer.getTicks() / 1000.0f; // Get time elapsed -- for time-based movement

    for(Particle& p : g_particles){ // Update all particles and render them
      p.Move(time); 
      p.Draw(render);
    }
    SDL_RenderPresent(render); // Display
  }

  SDL_DestroyRenderer(render);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

void handleEvent(SDL_Event event){ // TODO: lil messy
  /* 
    Variables to keep track of state 
      mouse_pressed : Is user holding down mouse? (For particle spawning)
      last_spawn_time : Used to manage particle spawn rate
      spawn_interval : Manage particle spawn rate
      zoom_level : For zooming
  */ 
  static bool mouse_pressed{false};
  static int last_spawn_time{0}; 
  static const int spawn_interval{50};
  static float zoom_level{1};

  if(event.type == SDL_QUIT){ // User request quit
    quit = true;
  }
  else if(event.type == SDL_KEYDOWN){ // Key was pressed
    switch(event.key.keysym.sym){
      case SDLK_SPACE:
        paused = !paused; // Pressing space will pause/unpause
        if(paused == false){
          timer.unpause(); // If timer gets unpaused, update timer
        }
        break;

      case SDLK_EQUALS:
        zoom_level += 0.1;
        SDL_RenderSetScale(render, zoom_level, zoom_level);
        break;
      
      case SDLK_MINUS:
        if(zoom_level > 1){
          zoom_level -= 0.1;
          SDL_RenderSetScale(render, zoom_level, zoom_level);
        }
        break;
    }
  }
  else if(event.type == SDL_MOUSEBUTTONDOWN && paused == false){ // Mouse was pressed and not currently paused
      int x, y;
      SDL_GetMouseState(&x, &y);
      if(x < 0 || x > SCREEN_W || y < 0 || y > SCREEN_H) return;
      Vector pos{static_cast<float>(x/zoom_level), static_cast<float>(y/zoom_level)};
      g_particles.push_back(Particle(G, pos));
      mouse_pressed = true;
  }
  else if (event.type == SDL_MOUSEBUTTONUP){ // Mouse released
      mouse_pressed = false;
  }
  else if (event.type == SDL_MOUSEMOTION){ // Mouse is moving
    if(mouse_pressed){
      Uint32 currentTime = SDL_GetTicks();
      if(currentTime - last_spawn_time >= spawn_interval){ // Spawn particles at specified rate
        int x, y;
        SDL_GetMouseState(&x, &y);
        if(x < 0 || x > SCREEN_W || y < 0 || y > SCREEN_H) return;
        Vector pos{static_cast<float>(x/zoom_level), static_cast<float>(y/zoom_level)};
        g_particles.push_back(Particle(static_cast<Colour>(x%3), pos));
        last_spawn_time = currentTime;
      }
    }
  }
}

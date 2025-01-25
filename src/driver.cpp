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

static SDL_Rect srcRect = {0, 0, SCREEN_W, SCREEN_H};  // Portion of the world to zoom into 

int main(int argc, char** argv){

  // Setup SDL window (the screen), render, and texture (for drawing particles)
  SDL_Window *window{nullptr};
  SDL_Texture *texture{nullptr};
  init(window, render, texture);

  SDL_Rect dstRect = { 0, 0, SCREEN_W, SCREEN_H};  // Fit the zoomed area to the screen
  SDL_Event event;

  int num_particles = std::stoi(argv[1]);

  // Equal amount of each colour
  for(int i = 0; i < num_particles; i++){ // TODO: scuffed
    g_particles.push_back(Particle(static_cast<Colour>(i%3)));
  }

  /* Game loop */
  while(quit == false){
    SDL_SetRenderTarget(render, texture); // render to the texture
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

    // After rendering to the texture, you can reset the target
    SDL_SetRenderTarget(render, NULL);  // Switch back to the default render target (the window)
    SDL_RenderCopy(render, texture, &srcRect, &dstRect);  // This will render the entire texture to the window
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
  static const int spawn_interval{50};
  static int last_spawn_time{0}; 
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
        srcRect = {SCREEN_W/4, SCREEN_H/4, SCREEN_W/2, SCREEN_H/2};
        break;
      
      case SDLK_MINUS:
        srcRect = {0, 0, SCREEN_W, SCREEN_H};
        break;

      case SDLK_LEFT:
        srcRect.x -= 10;
        break;
      
      case SDLK_RIGHT:
        srcRect.x += 10;
        break;

      case SDLK_UP:
        srcRect.y += 10;
        break;

      case SDLK_DOWN:
        srcRect.y -= 10;
        break;
    }
  }
  else if(event.type == SDL_MOUSEBUTTONDOWN && paused == false){ // Mouse was pressed and not currently paused
      int x, y;
      SDL_GetMouseState(&x, &y);
      // If mouse is not in screen, don't spawn
      if(x < 0 || x > SCREEN_W || y < 0 || y > SCREEN_H) return;
      // Get position for particle (scaled based on zoom level)
      Vector pos{static_cast<float>(x/zoom_level), static_cast<float>(y/zoom_level)}; // TODO: fix mouse scaling
      g_particles.push_back(Particle(G, pos)); // TODO: not rand
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
        // If mouse is not in screen, don't spawn
        if(x < 0 || x > SCREEN_W || y < 0 || y > SCREEN_H) return;
        // Get position for particle (scaled based on zoom level)
        Vector pos{static_cast<float>(x/zoom_level), static_cast<float>(y/zoom_level)}; // TODO: fix mouse scaling
        g_particles.push_back(Particle(static_cast<Colour>(x%3), pos)); // TODO: not rand
        last_spawn_time = currentTime;
      }
    }
  }
}

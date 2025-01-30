#include <iostream>
#include <vector>
#include <SDL.h>
#include <SDL_rect.h>
#include "../include/screen.hpp"
#include "../include/particle.hpp"
#include "../include/timer.hpp"


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

  // Used for zoom in/out. Fits the zoomed area to the screen
  SDL_Rect dstRect = { 0, 0, SCREEN_W, SCREEN_H};  

  int num_particles = std::stoi(argv[1]);

  // Generate particles
  for(int i = 0; i < num_particles; i++){ // TODO: scuffed
    g_particles.push_back(Particle(static_cast<Colour>(i%3)));
  }

  SDL_Event event;

  /* Game loop */
  while(quit == false){
    // Render to a texture (not directly to screen)
    SDL_SetRenderTarget(render, texture); 
    while(SDL_PollEvent(&event) != 0){ // Handle all user input (keyboard clicks, mouse actions)
      handleEvent(event);
    }
    
    // If paused, don't render
    if(paused) 
    continue;

    // Clean screen to prepare for next frame
    SDL_SetRenderDrawColor(render, 222, 216, 228, 255); 
    SDL_RenderClear(render);

    // Get time elapsed -- for time-based movement
    float time = timer.getTicks() / 1000.0f; 

    for(Particle& p : g_particles){ // Update all particles and render them
      p.Move(time); 
      p.Draw(render);
    }

    // After rendering to the texture, switch back to screen
    SDL_SetRenderTarget(render, NULL); 
    // Render to the screen based on zoom in/out (srcRect)
    SDL_RenderCopy(render, texture, &srcRect, &dstRect);  
    // Display
    SDL_RenderPresent(render); 
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

      // TODO: move this functionality to classes
      case SDLK_EQUALS: // Zoom in
        zoom_level += 0.1;

        // Scale srcRect
        srcRect.w = SCREEN_W / zoom_level;
        srcRect.h = SCREEN_H / zoom_level;

        /* 
           Zoom into centre of screen: this means that the centre of srcRect 
           (which is srcRect.x + srcRect.w/2 for the x dimension) needs to equal to 
           centre of screen. Move srcRect.w/2 to other side to get the needed srcRect.x.
           Same concept for y
        */
        srcRect.x = (SCREEN_W/2) - srcRect.w / 2;
        srcRect.y = (SCREEN_H/2) - srcRect.h / 2;
        break;
      
      case SDLK_MINUS: // Zoom out
        if(zoom_level == 1){
          break;
        }
        zoom_level -= 0.1;

        srcRect.w = SCREEN_W / zoom_level;
        srcRect.h = SCREEN_H / zoom_level;

        srcRect.x = (SCREEN_W/2) - srcRect.w / 2;
        srcRect.y = (SCREEN_H/2) - srcRect.h / 2;
        break;

      case SDLK_LEFT: // Pan left
        srcRect.x = (srcRect.x - 10 > 0) ? srcRect.x - 10 : 0;
        break;
      
      case SDLK_RIGHT: // Pan right
        srcRect.x = (srcRect.x + srcRect.w + 10 < SCREEN_W) ? srcRect.x + 10 : srcRect.x;
        break;

      case SDLK_UP: // Pan up
        srcRect.y = (srcRect.y - 10 > 0) ? srcRect.y - 10 : 0;
        break;

      case SDLK_DOWN: // Pan down
        srcRect.y = (srcRect.y + srcRect.h + 10 < SCREEN_H) ? srcRect.y + 10 : srcRect.y;
        break;
    }
  }
  else if(event.type == SDL_MOUSEBUTTONDOWN && paused == false){ // Mouse was pressed and not currently paused
      int x, y;
      SDL_GetMouseState(&x, &y);

      // If mouse is not in screen, don't spawn
      if(x < 0 || x > SCREEN_W || y < 0 || y > SCREEN_H) return;

      // Get position for the particle (scaled based on zoom level + panning)
      float mouseX = (x/zoom_level) + srcRect.x;
      float mouseY = (y/zoom_level) + srcRect.y;

      Vector pos{mouseX, mouseY};

      // Spawn
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

        // Get position for the particle (scaled based on zoom level + panning)
        float mouseX = (x/zoom_level) + srcRect.x;
        float mouseY = (y/zoom_level) + srcRect.y;

        Vector pos{mouseX, mouseY};

        // Spawn
        g_particles.push_back(Particle(static_cast<Colour>(x%3), pos)); // TODO: not rand
        last_spawn_time = currentTime;
      }
    }
  }
}

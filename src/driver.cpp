#include <iostream>
#include <vector>
#include <SDL.h>
#include <SDL_rect.h>
#include "../include/screen.h"
#include "../include/particle.h"
#include "../include/timer.h"

std::vector<Particle> g_particles;

int main(int argc, char** argv){

  // Setup SDL window and renderer 
  SDL_Window *window;
  SDL_Renderer *render;
  init(window, render);

  // Variables to keep track of state
  bool quit{false};
  bool paused{false};
  SDL_Event event;

  Timer timer;
  
  int num_particles = std::stoi(argv[1]);

  for(int i = 0; i < num_particles; i++){
    if(i%3==0)
    g_particles.push_back(Particle(R));
    else if (i%3 == 1)
    g_particles.push_back(Particle(G));
    else
    g_particles.push_back(Particle(B));
  }

  while(!quit){
    
      while(SDL_PollEvent(&event) != 0){
          // User request quit
          if(event.type == SDL_QUIT)
            quit = true;

          else if(event.type == SDL_KEYDOWN){
            // User pressed pause/play
            if(event.key.keysym.sym == SDLK_SPACE){
              paused = !paused; // Pressing space will pause/unpause
              if(paused == false){
                    timer.unpause(); // If paused then unpaused, update timer
              }
            }
          }
      }

      /* Game loop */
      if(paused)
        // don't render
        continue;

      // Clean screen
      SDL_SetRenderDrawColor(render, 222, 216, 228, 255); 
      SDL_RenderClear(render);

      float time = timer.getTicks() / 1000.0f; // Get time elapsed -- for time-based movement

      for(Particle& p : g_particles){
        p.Move(time);
        p.Draw(render);
      }
      SDL_RenderPresent(render);
  }

  SDL_DestroyRenderer(render);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}


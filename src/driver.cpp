#include <iostream>
#include <vector>
#include <SDL.h>
#include <SDL_rect.h>
#include "../include/screen.h"
#include "../include/particle.h"
#include "../include/timer.h"

#define NUM_PARTICLES 5
int main(){

    // Setup SDL window and renderer 
    SDL_Window *window;
    SDL_Renderer *render;
    init(window, render);

    bool quit{false};
    bool paused{false};
    SDL_Event event;

    std::vector<Particle> particles;
    Timer timer;

    for(int i = 0; i < NUM_PARTICLES; i++){
      particles.push_back(Particle(SCREEN_W/2, SCREEN_H/2));
    }

    while(!quit){
      
        while(SDL_PollEvent(&event) != 0){
            // User request quit
            if(event.type == SDL_QUIT)
              quit = true;

            else if(event.type == SDL_KEYDOWN){
              // User pause/play
              if(event.key.keysym.sym == SDLK_SPACE){
                paused = !paused;
                if(paused == false){
                      timer.unpause();
                }
              }
            }
        }

        /* Game loop */
        if(paused)
          // don't render
          continue;

        // Clean screen
        SDL_SetRenderDrawColor(render, 0, 0, 0, 255); // Black
        SDL_RenderClear(render);

        float time = timer.getTicks() / 1000.0f; // Get time elapsed -- for time-based movement

        for(Particle& p : particles){
          p.Move(time);
          p.Draw(render, 0, 255, 255);
        }
        SDL_RenderPresent(render);
    }

    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}


#include <iostream>
#include <SDL.h>
#include <SDL_rect.h>
#include "../include/screen.h"
#include "../include/particle.h"

int main(){

    // Setup SDL window and renderer 
    SDL_Window *window;
    SDL_Renderer *render;
    init(window, render);

    bool quit{false};
    SDL_Event e;

    Particle dude{SCREEN_W/2, SCREEN_H/2};
    while(!quit){
      
      while(SDL_PollEvent(&e) != 0){
        // User request quit
        if(e.type == SDL_QUIT){
          quit = true;
        }
      }

      /* Game loop */
      
      // Clean screen
      SDL_SetRenderDrawColor(render, 0, 0, 0, 255); // Black
      SDL_RenderClear(render);

      // Update render
      dude.Draw(render, 255, 0, 0);
      // Display it
      SDL_RenderPresent(render);
      dude.Move();
    }

    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}


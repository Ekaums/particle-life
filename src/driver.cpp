#include <iostream>
#include <vector>
#include <SDL.h>
#include <SDL_rect.h>
#include "../include/screen.h"
#include "../include/particle.h"
#include "../include/timer.h"

int main(){

    // Setup SDL window and renderer 
    SDL_Window *window;
    SDL_Renderer *render;
    init(window, render);

    bool quit{false};
    SDL_Event e;

    std::vector<Particle> particles;
    Timer timer;

    for(int i = 0; i < 100; i++){
      particles.push_back(Particle(SCREEN_W/2, SCREEN_H/2));
    }

    while(!quit){
      
        while(SDL_PollEvent(&e) != 0){
            // User request quit
            if(e.type == SDL_QUIT)
            quit = true;
        }

        /* Game loop */

        // Clean screen
        SDL_SetRenderDrawColor(render, 0, 0, 0, 255); // Black
        SDL_RenderClear(render);

        for(Particle& p : particles){
          p.Move();
          p.Draw(render, 255, 0, 0);
        }
        SDL_RenderPresent(render);
    }

    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}


#include <iostream>
#include <vector>
#include <SDL.h>
#include <SDL_rect.h>
#include "../include/screen.hpp"
#include "../include/particle.hpp"
#include "../include/timer.hpp"
#include "../include/simulation_state.hpp"



std::vector<Particle> g_particles;

int main(int argc, char** argv){

  // Setup SDL window (the screen), render, and texture (for drawing particles)
  SDL_Window *window{nullptr};
  SDL_Texture *texture{nullptr};
  SDL_Renderer *render; 
  init(window, render, texture);

  // Used for zoom in/out. Fits the zoomed area to the screen
  SDL_Rect dstRect = { 0, 0, SCREEN_W, SCREEN_H};  

  int num_particles = std::stoi(argv[1]);

  // Generate particles
  for(int i = 0; i < num_particles; i++){ // TODO: scuffed spawning
    g_particles.push_back(Particle(static_cast<Colour>(i%10)));
  }

  // Handles simulation input
  SimState sim_state{};

  /* Game loop */
  // TODO: encapsulate into fns
  while(sim_state.shouldQuit() == false){

    // TODO: quadtree impl

    // Handle all inputs
    sim_state.handleEvents();

    // Render to a texture (not directly to screen)
    SDL_SetRenderTarget(render, texture); 
    
    // If paused, don't render
    if(sim_state.isPaused()) 
    continue;

    // Clean screen to prepare for next frame
    SDL_SetRenderDrawColor(render, 0, 0, 0, 255); 
    SDL_RenderClear(render);

    // Get time elapsed -- for time-based movement
    float time = sim_state.getTicks() / 1000.0f; 

    for(Particle& p : g_particles){ // Update all particles and render them
      p.Move(time); 
      p.Draw(render);
    }

    // After rendering to the texture, switch back to screen
    SDL_SetRenderTarget(render, NULL); 
    // Render to the screen based on zoom in/out (srcRect)
    SDL_RenderCopy(render, texture, &sim_state.getSrcRect(), &dstRect);  
    // Display
    SDL_RenderPresent(render); 
  }

  SDL_DestroyRenderer(render);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

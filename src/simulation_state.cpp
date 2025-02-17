#include <SDL.h>
#include <vector>
#include "../include/simulation_state.hpp"
#include "../include/screen.hpp"
#include "../include/vector.hpp"
#include "../include/particle.hpp"

extern std::vector<Particle> g_particles;

// Main API: Handle all user input
void SimState::handleEvents(){
  SDL_Event event;

  while(SDL_PollEvent(&event) != 0){ // For all given inputs
    
    if(event.type == SDL_QUIT){ // Exit window
        quit = true;
    }
    else if(event.type == SDL_KEYDOWN){ // TODO: clean this up lol
      switch(event.key.keysym.sym){
        case SDLK_SPACE: // Pressing space will pause/unpause
          togglePause();
          break;

        case SDLK_EQUALS: // Zoom in
          handleZoom(0.1);
          break;
        
        case SDLK_MINUS: // Zoom out
          handleZoom(-0.1);
          break;
        
        case SDLK_LEFT:
          panScreen(Left);
          break;

        case SDLK_RIGHT:
          panScreen(Right);
          break;
        
        case SDLK_UP:
          panScreen(Up);
          break;
        
        case SDLK_DOWN:
          panScreen(Down);
          break;
        
        case SDLK_1:
            setParticleColour(Colour::Red);
            break;
        
        case SDLK_2:
            setParticleColour(Colour::Green);
            break;
        case SDLK_3:
            setParticleColour(Colour::Blue);
            break;
        
        case SDLK_4:
            setParticleColour(Colour::Yellow);
            break;

        case SDLK_5:
            setParticleColour(Colour::Purple);
            break;

        case SDLK_6:
            setParticleColour(Colour::Pink);
            break;

        case SDLK_7:
            setParticleColour(Colour::Orange);
            break;

        case SDLK_8:
            setParticleColour(Colour::Magenta);
            break;

        case SDLK_9:
            setParticleColour(Colour::Aqua);
            break;

        case SDLK_0:
            setParticleColour(Colour::Teal);
            break;
      }
    }
    else if(event.type == SDL_MOUSEBUTTONDOWN && paused == false){ // If Left click, spawn a particle
      spawnParticle();
      mouse_pressed = true;
    }
    else if(event.type == SDL_MOUSEBUTTONUP){
      mouse_pressed = false;
    }
    else if(event.type == SDL_MOUSEMOTION){
      if(mouse_pressed){
        // If mouse is still pressed while moving, spawn particles at mouse pos
        current_time = SDL_GetTicks();
        if(current_time - last_spawn_time >= spawn_interval){ // Spawn at a rate
          spawnParticle();
          last_spawn_time = current_time;
        }
      }
    }
  }
}

void SimState::togglePause(){
  paused = !paused; 
  if(paused == false){
    timer.unpause(); // If timer gets unpaused, update timer
  }
}

void SimState::handleZoom(float zoom){
  if(zoom_level < 0.2 || zoom_level > 4.5){ // TODO: magic nums
    return;
  }

  float old_zoom = zoom_level;
  zoom_level += zoom;

  // Scale src_rect
  src_rect.w = SCREEN_W / zoom_level;
  src_rect.h = SCREEN_H / zoom_level;

  /* 
      Zoom into centre of screen: this means that the centre of src_rect 
      (which is src_rect.x + src_rect.w/2 for the x dimension) needs to equal to 
      centre of screen. Move src_rect.w/2 to other side to get the needed src_rect.x.
      Same concept for y
  */
  int x, y;
  SDL_GetMouseState(&x, &y);
  x = (x/old_zoom) + src_rect.x;
  y= (y/old_zoom) + src_rect.y;


  src_rect.x = x - src_rect.w / 2;
  src_rect.y = y - src_rect.h / 2;
  src_rect.x = std::clamp(src_rect.x, 0, WORLD_W-src_rect.w);
  src_rect.y = std::clamp(src_rect.y, 0, WORLD_H-src_rect.h);
}

void SimState::panScreen(Direction dir){
  switch(dir){ 
    case Left:
      src_rect.x = (src_rect.x - 10 > 0) ? src_rect.x - 10 : 0;
      break;

    case Right:
      src_rect.x = (src_rect.x + src_rect.w + 10 < WORLD_W) ? src_rect.x + 10 : src_rect.x;
      break;

    case Up:
      src_rect.y = (src_rect.y - 10 > 0) ? src_rect.y - 10 : 0;
      break;
    
    case Down:
      src_rect.y = (src_rect.y + src_rect.h + 10 < WORLD_H) ? src_rect.y + 10 : src_rect.y;
      break;
  }
}


void SimState::spawnParticle(){
  int x, y;
  SDL_GetMouseState(&x, &y);

  // If mouse is not in screen, don't spawn
  if(x < 0 || x > SCREEN_W || y < 0 || y > SCREEN_H) return;

  // Get position for the particle (scaled based on zoom level + panning)
  float mouseX = (x/zoom_level) + src_rect.x;
  float mouseY = (y/zoom_level) + src_rect.y;

  Vector pos{mouseX, mouseY};

  // Spawn
  g_particles.push_back(Particle(spawn_colour, pos)); // TODO: not rand
}
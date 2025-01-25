#include <SDL.h>
#include <SDL_rect.h>
#include <iostream>
#include "../include/screen.h"

void init(SDL_Window* &window, SDL_Renderer* &render, SDL_Texture* &texture){
  // Init
  if(SDL_Init(SDL_INIT_VIDEO) < 0){
      std::cout << "Could not initialize. Error: " << SDL_GetError() << '\n'; // GetError tells you any errors from SDL
      exit(1); 
  }

  // Create window
  window = SDL_CreateWindow("Particle Life", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_W, SCREEN_H, SDL_WINDOW_SHOWN);
  if(window == nullptr){
      std::cout << " Window could not be created. Error: " << SDL_GetError() << '\n';
      exit(1); 
  }

  // Get window surface to manipulate
  render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (!render) {
      std::cout << "Renderer could not be created SDL_Error: " << SDL_GetError() << '\n';
      exit(1);
  }

  // Create texture (to draw all particles on)
  texture = SDL_CreateTexture(render, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, SCREEN_W, SCREEN_H);
}

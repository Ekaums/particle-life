#pragma once

#include <SDL.h>
#include <SDL_rect.h>

// Dimensions of the actual screen
const int SCREEN_W{1240};
const int SCREEN_H{800};

// Dimensions of the simulation
const int WORLD_W{5000};
const int WORLD_H{5000};

// Init SDL window
void init(SDL_Window* &window, SDL_Renderer* &render, SDL_Texture* &texture);


#pragma once

#include <SDL.h>
#include <SDL_rect.h>

const int SCREEN_W{1240};
const int SCREEN_H{800};

// Init SDL window
void init(SDL_Window* &window, SDL_Renderer* &render, SDL_Texture* &texture);


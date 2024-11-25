#pragma once

#include <SDL.h>
#include <SDL_rect.h>

constexpr int SCREEN_W{1240};
constexpr int SCREEN_H{800};

void init(SDL_Window * &window, SDL_Renderer * &render);


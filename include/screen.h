#pragma once

#include <SDL.h>
#include <SDL_rect.h>

static const int SCREEN_W{1240};
static const int SCREEN_H{800};

void init(SDL_Window * &window, SDL_Renderer * &render);


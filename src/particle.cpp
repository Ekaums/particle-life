#include <random>
#include <SDL.h>
#include <SDL_rect.h>
#include <iostream>
#include "../include/particle.h"

Particle::Particle(int x, int y){
  pos.x = x;
  pos.y = y;

  // Rand number between -3 and 3
  std::random_device rd;  // Seed
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distrib(-3, 3);

  velocity.x = distrib(gen);
  velocity.y = distrib(gen);
}

void Particle::Move(){
  pos.x += velocity.x;
  pos.y += velocity.y;
}

void Particle::Draw(SDL_Renderer *render, int r, int g, int b){
  
  // Set colour for particle
  SDL_SetRenderDrawColor(render, r, g, b, 255);
  SDL_Rect particleRect = {pos.x, pos.y, 10, 10};
  SDL_RenderFillRect(render, &particleRect);
}
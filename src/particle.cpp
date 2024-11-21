#include <random>
#include <SDL.h>
#include <SDL_rect.h>
#include <iostream>
#include "../include/particle.h"
#include "../include/screen.h"

Particle::Particle(float x, float y){
  pos.x = x;
  pos.y = y;

  // Random values
  std::random_device rd;  // Seed
  std::mt19937 gen(rd());
  std::uniform_real_distribution<float> distrib(-600, 600);
  float random = distrib(gen);

  velocity.x = random;
  velocity.y = random;
}

void Particle::Move(float Timestep){
  pos.x += velocity.x * Timestep;
  pos.y += velocity.y * Timestep;

  if(pos.x < 0){
    pos.x = 0;
    velocity.x *= -1;
  }
  else if(pos.x > SCREEN_W){
    pos.x = SCREEN_W;
    velocity.x *= -1;
  }

  if(pos.y < 0){
    pos.y = 0;
    velocity.y *= -1;
  }
  else if(pos.y > SCREEN_H){
    pos.y = SCREEN_H;
    velocity.y *= -1;
  }
  
}

void Particle::Draw(SDL_Renderer * &render, int r, int g, int b){
  
  // Set colour for particle
  SDL_SetRenderDrawColor(render, r, g, b, 255);
  // Generate particle
  SDL_Rect particleRect = {static_cast<int>(pos.x), static_cast<int>(pos.y), 10, 10};
  // Draw
  SDL_RenderFillRect(render, &particleRect);
}
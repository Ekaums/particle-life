#include <random>
#include <SDL.h>
#include <SDL_rect.h>
#include <iostream>
#include "../include/particle.h"
#include "../include/screen.h"

Particle::Particle(float x, float y){
  // Random values
  std::random_device rd;  // Seed
  std::mt19937 gen(rd());
  std::uniform_real_distribution<float> distrib(-600, 600);

  pos.x = distrib(gen);
  pos.y = distrib(gen);

  velocity.x = distrib(gen);
  velocity.y = distrib(gen);
}

void Particle::Move(float time){
  
  pos.x += velocity.x * time;
  pos.y += velocity.y * time;

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
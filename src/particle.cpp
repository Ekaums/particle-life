#include <random>
#include <SDL.h>
#include <SDL_rect.h>
#include <iostream>
#include "../include/particle.h"
#include "../include/screen.h"
#include "../include/vector.h"

extern std::vector<Particle> particles;

Particle::Particle(){
  // Each particle has random position, velocity, and acceleration
  pos.rand(0, SCREEN_H); // TODO: scuffed random position
  vel.rand(-155, 155);
  acc.randNorm();
}

void Particle::Move(float time){
  
  // Each frame, give particle random acceleration
  acc.randNorm();
  vel.update(acc, time);

  // Max velocity
  if(vel.x > 155){
    vel.x = 155;
  }
  else if(vel.x < -155){
    vel.x = -155;
  }
  if(vel.y > 155){
    vel.y = 155;
  }
  else if(vel.y < -155){
    vel.y = -155;
  }


  pos.update(vel, time);
  
  // Keep particle within window
  if(pos.x < 0){
    pos.x = 0;
    vel.x *= -1;
  }
  else if(pos.x > SCREEN_W){
    pos.x = SCREEN_W;
    vel.x *= -1;
  }
  if(pos.y < 0){
    pos.y = 0;
    vel.y *= -1;
  }
  else if(pos.y > SCREEN_H){
    pos.y = SCREEN_H;
    vel.y *= -1;
  }

  checkCollision(*this, particles);
}

void Particle::Draw(SDL_Renderer * &render, int r, int g, int b){
  
  // Set colour for particle
  SDL_SetRenderDrawColor(render, r, g, b, 255);
  // Generate particle
  SDL_Rect particleRect = {static_cast<int>(pos.x), static_cast<int>(pos.y), PARTICLE_SIZE, PARTICLE_SIZE};
  // Draw
  SDL_RenderFillRect(render, &particleRect);
}

void Particle::checkCollision(Particle &p, std::vector<Particle>& particles){

  for(Particle& par : particles){
    if(&par == &p){ // Comparing objects by address
      continue;
    }

    Vector distance = p.pos - par.pos;

    // if touching:
    if(abs(distance.x) < PARTICLE_SIZE && abs(distance.y) < PARTICLE_SIZE){
      std::cout << "HIT\n";

      distance.Norm();
      Vector vel = p.vel - par.vel;
      Vector impulse = distance * vel.Dot(distance);

      p.vel.x += impulse.x;
      p.vel.y += impulse.y;

      par.vel.x -= impulse.x;
      par.vel.y -= impulse.y;
    }

  }
}
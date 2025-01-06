#include <random>
#include <SDL.h>
#include <SDL_rect.h>
#include <cmath>
#include <iostream>
#include "../include/particle.h"
#include "../include/screen.h"
#include "../include/vector.h"

extern std::vector<Particle> particles;

Particle::Particle(){
  // Each particle has random position, velocity, and acceleration
  pos.rand(0, SCREEN_W); // TODO: scuffed random position
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
  // pos is the top-left coordinates of the square (from which it is drawn)
  if(pos.x < 0){
    pos.x = 0;
    vel.x *= -1;
  }
  else if(pos.x + PARTICLE_SIZE > SCREEN_W){
    pos.x = SCREEN_W - PARTICLE_SIZE;
    vel.x *= -1;
  }
  if(pos.y < 0){
    pos.y = 0;
    vel.y *= -1;
  }
  else if(pos.y + PARTICLE_SIZE > SCREEN_H){
    pos.y = SCREEN_H - PARTICLE_SIZE;
    vel.y *= -1;
  }

  // Handle any particle collisions
  resolveCollisions(*this, particles);
}

void Particle::Draw(SDL_Renderer* &render, int r, int g, int b){
  // Set colour for particle
  SDL_SetRenderDrawColor(render, r, g, b, 255);
  // Generate particle
  SDL_Rect particleRect = {static_cast<int>(pos.x), static_cast<int>(pos.y), PARTICLE_SIZE, PARTICLE_SIZE};
  // Draw
  SDL_RenderFillRect(render, &particleRect);
}

void Particle::resolveCollisions(Particle &p, std::vector<Particle>& particles){
  for(Particle& otherP : particles){

    if(&otherP == &p){ // If same particle, skip 
      continue;
    }

    // SDL draws shapes starting from the top-left. So the centre of each square is found here
    Vector centreOfP = p.pos + Vector(PARTICLE_SIZE / 2, PARTICLE_SIZE / 2);
    Vector centreOfOtherP = otherP.pos + Vector(PARTICLE_SIZE / 2, PARTICLE_SIZE / 2);
    Vector distance = centreOfP - centreOfOtherP;

    // How much the particles overlap in each axis
    float overlapX = PARTICLE_SIZE - std::abs(distance.x);
    float overlapY = PARTICLE_SIZE - std::abs(distance.y);

    // If they are touching, there is collision!!
    if(overlapX > 0 && overlapY > 0){ // TODO: use AABB instead

      /* Compute new particle velocities */

      Vector normal;
      /* 
        Normal vector points from otherP to p
        Normal vector is on the axis where the collision force is applied
        Since the particles are axis-aligned squares, the contact point always occurs using the sides of squares which are all 1-dimensional (just in X or Y dimension)! 
        Meaning the collision vector is always in 1 dimension, so the force is applied in just X or Y
        The direction of normal vector is in the dimension of smaller overlap
      */
      if (overlapX < overlapY){
        normal = Vector(distance.x > 0 ? 1 : -1, 0); // X-axis normal
      } else{
        normal = Vector(0, distance.y > 0 ? 1 : -1); // Y-axis normal
      }

      // Velocity of p relative to otherP (from perspective of otherP)
      Vector RelativeVel = p.vel - otherP.vel;

      // Get the component of relative velocity that is in the dimension where force wil be applied (dot product), and scale normal vector by that amount
      // Impulse = change in momentum (just velocity here since same mass) of particles
      Vector impulse = normal * RelativeVel.Dot(normal); 

      // Collision normal points from otherP to p, otherP is in same direction of impulse (add) and p is opposite direction of impulse (subtract)
      otherP.vel += impulse;
      p.vel -= impulse;

      /* 
        Shift particle positions so they are no longer colliding. 
        This is needed such that multiple collisions aren't detected at once.
        Particles are shifted in either X or Y dimension, depending on which shift would be smaller (more negligible)
      */

      if(overlapX < overlapY){
        if(distance.x > 0) { // p is to the right of otherP
          p.pos.x += overlapX/2; 
          otherP.pos.x -= overlapX/2;
        } else{ // p is to the left of otherP
          p.pos.x -= overlapX/2; 
          otherP.pos.x += overlapX/2;
        }
      } else{
        if(distance.y > 0) { // p is below otherP
          p.pos.y += overlapY/2; 
          otherP.pos.y -= overlapY/2;
        } else{ // p is above otherP
          p.pos.y -= overlapY/2; 
          otherP.pos.y += overlapY/2; 
        }
      }
    }
  }
}
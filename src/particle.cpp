#include <random>
#include <SDL.h>
#include <SDL_rect.h>
#include <cmath>
#include <iostream>
#include "../include/particle.h"
#include "../include/screen.h"
#include "../include/vector.h"

extern std::vector<Particle> particles;

Particle::Particle(Colour c){
  // Each particle has random position, velocity, and acceleration
  pos.rand(0, SCREEN_W); // TODO: scuffed random position
  vel.rand(-155, 155); // TODO: magic num
  acc.randNorm();
  acc *= 100; // Scale acc // TODO: scuffed

  // Also has a type (depending on colour)
  col = c;
}

void Particle::Move(float time){
  // Each frame, give particle random acceleration
  acc.randNorm();
  acc *= 100; // TODO: scuffed scaling
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

  // Handle any physical collisions
  resolveCollisions(*this, particles);

  // Handle any magic forces
  resolveForces(*this, particles);
}

void Particle::Draw(SDL_Renderer* &render){
  // Set colour for particle
  // TODO: use a map or smthng instead of switch case
  switch (col){
    case R:
      SDL_SetRenderDrawColor(render, 255, 128, 128, 255);
      break;
    case B:
      SDL_SetRenderDrawColor(render, 100, 149, 237, 255);
      break;
  }
  // Generate particle
  SDL_Rect particleRect = {static_cast<int>(pos.x), static_cast<int>(pos.y), PARTICLE_SIZE, PARTICLE_SIZE};
  // Draw
  SDL_RenderFillRect(render, &particleRect);
}

void Particle::resolveCollisions(Particle &p, std::vector<Particle>& particles){
  for(Particle& otherP : particles){

    if(&p == &otherP){ // If same particle, skip 
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


void Particle::resolveForces(Particle &p, std::vector<Particle>& particles){
  bool attract{false};

  for(Particle& otherP : particles){
  
    if(&p == &otherP) // If same particle, skip
      continue;

    attract = false;

    if(p.col == otherP.col){ // If same colour, attract
      attract = true;
      //continue;
    }
    

    // SDL draws shapes starting from the top-left. So the centre of each square is found here
    Vector centreOfP = p.pos + Vector(PARTICLE_SIZE / 2, PARTICLE_SIZE / 2);
    Vector centreOfOtherP = otherP.pos + Vector(PARTICLE_SIZE / 2, PARTICLE_SIZE / 2);
    Vector distance = centreOfP - centreOfOtherP;

    if(distance.x > 25 || distance.y > 25){ // OR I can do opposite check and check if both are within distance value (and do forces)
      continue; // Too far
    }
    // TODO: too close (avoid divide by 0)

    float magnitude = std::sqrt(distance.x * distance.x + distance.y * distance.y);
    distance.Normalize(); // Normal vector now (prolly change)
    float strength = 1000;

    float forceMag = strength / (magnitude * magnitude);
    Vector force = distance * forceMag;
    
    if(attract){
      otherP.vel += force;
      p.vel -= force;
    }
    else{
      otherP.vel -= force;
      p.vel += force;
    }
  }
}
#include <random>
#include <SDL.h>
#include <SDL_rect.h>
#include <cmath>
#include <iostream>
#include "../include/particle.h"
#include "../include/screen.h"
#include "../include/vector.h"

extern std::vector<Particle> g_particles;
const int particleSize{6};
const float maxVel{105};

Particle::Particle(Colour c){
  // Each particle has random position, velocity, and acceleration
  pos.rand(0, SCREEN_W); // TODO: scuffed random position
  vel.rand(-maxVel, maxVel); 
  acc.randNorm();
  acc *= 100; // Scale acc // TODO: scuffed

  // Also has a type (depending on colour)
  col = c;
}

Particle::Particle(Colour c, Vector pos){

  // Create particle with specified position 
  this->pos = pos;
  vel.rand(-maxVel, maxVel); 
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
  // Limit max velocity
  vel.x = std::clamp(vel.x, -maxVel, maxVel);
  vel.y = std::clamp(vel.y, -maxVel, maxVel);

  pos.update(vel, time);
  // Particles will reappear on other side of screen if they move past border
  if(pos.x + particleSize < 0){
    pos.x = SCREEN_W;
  }
  else if(pos.x > SCREEN_W){
    pos.x = 0;
  }
  if(pos.y + particleSize < 0){
    pos.y = SCREEN_H;
  }
  else if(pos.y > SCREEN_H){
    pos.y = 0;
  }

  // Handle any physical collisions
  resolveCollisions(*this, g_particles);
  // Handle any attract/repel forces
  resolveForces(*this, g_particles);
}

void Particle::Draw(SDL_Renderer* &render){
  // Set colour for particle
  // TODO: use a map or smthng instead of switch case
  switch (col){
    case R:
      SDL_SetRenderDrawColor(render, 255, 128, 128, 255);
      break;

    case G:
      SDL_SetRenderDrawColor(render, 0, 168, 107, 255);
      break;

    case B:
      SDL_SetRenderDrawColor(render, 100, 149, 237, 255);
      break;
  }
  // Generate particle
  SDL_Rect particleRect = {static_cast<int>(pos.x), static_cast<int>(pos.y), particleSize, particleSize};
  // Draw
  SDL_RenderFillRect(render, &particleRect);
}

void Particle::resolveCollisions(Particle &p, std::vector<Particle>& particles){
  for(Particle& otherP : particles){

    if(&p == &otherP){ // If same particle, skip 
      continue;
    }

    // SDL draws shapes starting from the top-left. So the centre of each square is found here
    Vector centreOfP = p.pos + Vector(particleSize / 2, particleSize / 2);
    Vector centreOfOtherP = otherP.pos + Vector(particleSize / 2, particleSize / 2);
    Vector distance = centreOfP - centreOfOtherP;

    // How much the particles overlap in each axis
    float overlapX = particleSize - std::abs(distance.x);
    float overlapY = particleSize - std::abs(distance.y);

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
  for(Particle& otherP : particles){

    if(&p == &otherP) // If same particle, skip
      continue;

    // SDL draws shapes starting from the top-left. So the centre of each square is found here
    Vector centreOfP = p.pos + Vector(particleSize / 2, particleSize / 2);
    Vector centreOfOtherP = otherP.pos + Vector(particleSize / 2, particleSize / 2);
    Vector distance = centreOfP - centreOfOtherP;

    if(distance.x > 35 || distance.y > 35){ // TODO: OR I can do opposite check and check if both are within distance value (and do forces)
      continue; // Too far
    }
    // TODO: too close (avoid divide by 0) (idk if needed)

    float magnitude = std::sqrt(distance.x * distance.x + distance.y * distance.y);
    distance.Normalize(); // Normal vector now (prolly change)
    float strength = 1000; // TODO: magic number

    float forceMag = strength / (magnitude * magnitude);
    Vector force = distance * forceMag;
    // Force points from otherP to p

    // Compute attract/repel force depending on config
    float interactionP = InteractionMatrix[p.col][otherP.col];
    float interactionOtherP = InteractionMatrix[otherP.col][p.col];

    // Apply forces
    otherP.vel += force * interactionOtherP;
    p.vel -= force * interactionP;
  }
}
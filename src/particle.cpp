#include <random>
#include <SDL.h>
#include <SDL_rect.h>
#include <cmath>
#include <iostream>
#include "../include/particle.hpp"
#include "../include/screen.hpp"
#include "../include/vector.hpp"

extern std::vector<Particle> g_particles;

Particle::Particle(Colour c) : col(c){

  // Each particle has random position, velocity, and acceleration
  pos.rand(0, WORLD_W); // TODO: scuffed random position
  vel.rand(-maxVel, maxVel); 
  acc.randNorm();
  acc *= accScale;
}

Particle::Particle(Colour c, Vector pos) : col(c){

  // Create particle with specified position 
  this->pos = pos;
  vel.rand(-maxVel, maxVel); 
  acc.randNorm();
  acc *= accScale; // Scale acc
}

void Particle::Move(float time){
  // Each frame, give particle random acceleration
  acc.randNorm();
  acc *= accScale; // Scale acc
  vel.update(acc, time);
  // Limit max velocity
  vel.x = std::clamp(vel.x, -maxVel, maxVel);
  vel.y = std::clamp(vel.y, -maxVel, maxVel);

  pos.update(vel, time);
  // Particles spawn based on world dimensions (which are larger than viewable screen) 
  if(pos.x + particleSize < 0){
    pos.x = WORLD_W;
  }
  else if(pos.x > WORLD_W){
    pos.x = 0;
  }
  if(pos.y + particleSize < 0){
    pos.y = WORLD_H;
  }
  else if(pos.y > WORLD_H){
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
    case Colour::Red:
      SDL_SetRenderDrawColor(render, 230, 92, 92, 255);
      break;

    case Colour::Green:
      SDL_SetRenderDrawColor(render, 120, 200, 120, 255);
      break;

    case Colour::Blue:
      SDL_SetRenderDrawColor(render, 86, 156, 214, 255);
      break;

    case Colour::Yellow:
      SDL_SetRenderDrawColor(render, 247, 220, 111, 255);
      break;
    
    case Colour::Purple:
      SDL_SetRenderDrawColor(render, 161, 119, 202, 255);
      break;

    case Colour::Pink:
      SDL_SetRenderDrawColor(render, 240, 150, 170, 255);
      break;
    
    case Colour::Orange:
      SDL_SetRenderDrawColor(render, 255, 165, 100, 255);
      break;
    
    case Colour::Magenta:
      SDL_SetRenderDrawColor(render, 200, 100, 180, 255);
      break;

    case Colour::Aqua:
      SDL_SetRenderDrawColor(render, 100, 200, 200, 255);
      break;

    case Colour::Teal:
      SDL_SetRenderDrawColor(render, 72, 150, 140, 255);
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

    float forceMag = forceStrength / (magnitude * magnitude);
    Vector force = distance * forceMag;
    // Force points from otherP to p

    // Compute attract/repel force depending on config
    float interactionP = InteractionMatrix[p.col][static_cast<int>(otherP.col)];
    float interactionOtherP = InteractionMatrix[otherP.col][static_cast<int>(p.col)];

    // Apply forces
    otherP.vel += force * interactionOtherP;
    p.vel -= force * interactionP;
  }
}
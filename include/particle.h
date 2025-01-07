#pragma once 

#include "../include/vector.h"
#define PARTICLE_SIZE 10

enum Colour{
  R,
  B
};

class Particle{

  private:
    // Each particle has position, velocity, and acceleration
    Vector pos, vel, acc;
    // And type
    Colour col;

  public:
    Particle(Colour c);

    // Update particle's movement based on time elapsed
    void Move(float time);

    // Handles physical collisions between particles
    static void resolveCollisions(Particle &p, std::vector<Particle>& particles);
    
    // Handle any magic forces
    static void resolveForces(Particle &p, std::vector<Particle>& particles);

    // Render particle
    void Draw(SDL_Renderer* &render);
};
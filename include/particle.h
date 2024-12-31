#pragma once 

#include "../include/vector.h"
#define PARTICLE_SIZE 10

class Particle{

  private:
    // Each particle has position, velocity, and acceleration
    Vector pos, vel, acc;

  public:
    Particle();

    // Update particle's movement based on time elapsed
    void Move(float time);

    static void checkCollision(Particle &p, std::vector<Particle>& particles);

    // Render particle
    void Draw(SDL_Renderer * &render, int r, int g, int b);
};
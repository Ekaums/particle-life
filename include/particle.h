#pragma once 

#include "../include/vector.h"

class Particle{

  private:
    // Each particle has position, velocity, and acceleration
    Vector pos, vel, acc;

  public:
    Particle();

    // Update particle's movement based on time elapsed
    void Move(float time);

    // Render particle
    void Draw(SDL_Renderer * &render, int r, int g, int b);
};
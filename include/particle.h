#pragma once 

#include "../include/timer.h" // TODO: is this necessary? better way to organize?

typedef struct Vector{
  float x;
  float y;
} Vector;

class Particle{

  private:
    Vector velocity;
    // Vector pos;

  public:

    Vector pos;
    Timer timer;
    Particle(float x, float y);

    // Update particle pos
    void Move();

    // Render particle
    void Draw(SDL_Renderer * &render, int r, int g, int b);
};
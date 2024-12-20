#pragma once 

#include "../include/timer.h" // TODO: is this necessary? better way to organize?

typedef struct Vector{
  float x;
  float y;

  void update(Vector u, float time);

} Vector;

class Particle{

  private:
    Vector velocity;
    Vector acceleration;
    // Vector pos;

  public:

    Vector pos;
    Particle(float x, float y);

    // Update particle pos
    void Move(float time);

    // Render particle
    void Draw(SDL_Renderer * &render, int r, int g, int b);
};
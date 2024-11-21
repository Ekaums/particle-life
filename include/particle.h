#pragma once 

typedef struct Vector{
  float x;
  float y;
} Vector;

class Particle{

  private:
    Vector velocity;

  public:

    Vector pos;
    Particle(float x, float y);

    // Update particle pos
    void Move(float Timestep);

    // Render particle
    void Draw(SDL_Renderer * &render, int r, int g, int b);
};
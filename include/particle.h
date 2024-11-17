#pragma once 

typedef struct Vector{
  int x;
  int y;
} Vector;

class Particle{

  private:
    Vector pos, velocity;

  public:
    Particle(int x, int y);

    // Update particle pos
    void Move();

    // Render particle
    void Draw(SDL_Renderer *render, int r, int g, int b);

};
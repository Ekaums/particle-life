#pragma once 

#include "../include/vector.h"

enum Colour{
  R,
  G,
  B
};

class Particle{

  private:
    // Each particle has position, velocity, and acceleration
    Vector pos, vel, acc;
    // And type
    Colour col;

    // Interaction between different colours
    // Since this is static and the definition is provided here, it must be inlined
    inline static const std::vector<std::vector<float>> InteractionMatrix = {
    // R  G  B
    { -1.0f,  -1.0f,  -1.0f }, 
    {  -1.0f, -1.0f,  -1.0f }, 
    {  -1.0f,  -1.0f, -1.0f }  
    // Example: All particles repel same kind, attract each other
    };

  public:
    // Create particle of specified type with random attributes (pos,vel,acc)
    Particle(Colour c);

    // Create particle with specific position
    Particle(Colour c, Vector pos);

    // Update particle's movement based on time elapsed
    void Move(float time);

    // Handles physical collisions between particles
    static void resolveCollisions(Particle &p, std::vector<Particle>& particles);
    
    // Handle any magic forces
    static void resolveForces(Particle &p, std::vector<Particle>& particles);

    // Render particle
    void Draw(SDL_Renderer* &render);
};
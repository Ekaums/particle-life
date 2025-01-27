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
    const Colour col;

    // Interaction between different types
    // Since this is static and the definition is provided here, it must be inlined
    inline static const std::vector<std::vector<float>> InteractionMatrix = {
      // R  G  B
      { -0.5f,  1.0f,  -1.0f }, 
      {  -1.0f, -0.5f,  1.0f }, 
      {  1.0f,  -1.0f, -0.5f }  
    };

    // Particle attributes
    static const int particleSize{5};
    static const int accScale{100};
    static const int forceStrength{1000};
    // Non-integral types can't be initalized in class declaration (therefore need to be inlined)
    inline static const float maxVel{115};

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
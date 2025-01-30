#pragma once 

#include "../include/vector.hpp"

enum Colour{
  Red,
  Green,
  Blue,
  Yellow,
  Purple,
  Pink,
  Orange,
  Magenta,
  Aqua,
  Teal
};

class Particle{

  private:
    // Each particle has position, velocity, and acceleration
    Vector pos, vel, acc;

    // And type
    const Colour col;

    // Interaction between different types
    // Since this is static and the definition is provided here, it must be inlined
    inline static const float self = 0.9f;
    inline static const float attract = 0.7f;
    inline static const float repel = -0.7f;

    inline static const std::vector<std::vector<float>> InteractionMatrix = {
        { self,   attract, repel,   0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f   }, // Red
        { repel,  self,    attract, 0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f    }, // Blue
        { 0.0f,   repel,   self,    attract, 0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f    }, // Yellow
        { 0.0f,   0.0f,    repel,   self,    attract, 0.0f,    0.0f,    0.0f,    0.0f,    0.0f    }, // Green
        { 0.0f,   0.0f,    0.0f,    repel,   self,    attract, 0.0f,    0.0f,    0.0f,    0.0f    }, // Purple
        { 0.0f,   0.0f,    0.0f,    0.0f,    repel,   self,    attract, 0.0f,    0.0f,    0.0f    }, // Pink
        { 0.0f,   0.0f,    0.0f,    0.0f,    0.0f,    repel,   self,    attract, 0.0f,    0.0f    }, // Orange
        { 0.0f,   0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    repel,   self,    attract, 0.0f    }, // Magenta
        { 0.0f,   0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    repel,   self,    attract }, // Aqua
        { attract, 0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    repel,   self    }  // Teal
    };


    // Particle attributes
    static const int particleSize{6};
    static const int accScale{100};
    static const int forceStrength{1000};
    // Non-integral types can't be initalized in class declaration (therefore need to be inlined)
    inline static const float maxVel{65};

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
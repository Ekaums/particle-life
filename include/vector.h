#pragma once

// My own vector
typedef struct Vector{
  // 2D
  float x;
  float y;

  // Update position/velocity based on time elapsed
  void update(Vector otherVector, float time);
  
  // Generate a random vector based on bounds [a, b]
  void rand(float a, float b);

  // Generate a random unit vector
  void randNorm();
} Vector;


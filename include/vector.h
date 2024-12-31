#pragma once

// My own vector
typedef struct Vector{
  // 2D
  float x;
  float y;

  // Update position/velocity based on time elapsed
  void update(Vector &otherVector, float time);
  
  Vector operator-(const Vector &v1){
    return {this->x - v1.x, 
            this->y - v1.y};
  }

    Vector operator*(float val){
    return {this->x * val, 
            this->y * val};
  }

  // Generate a random vector based on bounds [a, b]
  void rand(float a, float b);

  void Norm();

  float Dot(const Vector &o);

  // Generate a random unit vector
  void randNorm();
} Vector;


#pragma once

// My own 2D physics vector
typedef struct Vector{
  // 2D
  float x;
  float y;

  // Default constructor
  Vector() : x(0), y(0) {}
  // Custom constructor
  Vector(float x1, float y1) : x(x1), y(y1) {}

  // Update position/velocity based on time elapsed
  void update(Vector &otherVector, float time);

  Vector operator+(const Vector &v1) const{
    return {this->x + v1.x, 
            this->y + v1.y};
  }

  Vector operator-(const Vector &v1) const{
    return {this->x - v1.x, 
            this->y - v1.y};
  }

  void operator+=(const Vector &v1){
    this->x += v1.x;
    this->y += v1.y;
  }

  void operator-=(const Vector &v1){
    this->x -= v1.x;
    this->y -= v1.y;
  }

  Vector operator*(float n) const{
    return {this->x * n, 
            this->y * n};
  }

  // Generate a random vector based on bounds [a, b]
  void rand(float a, float b);

  float Dot(const Vector &o) const;

  void Norm();

  // Generate a random unit vector
  void randNorm();
} Vector;


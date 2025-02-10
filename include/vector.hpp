#pragma once

// My own 2D physics vector
struct Vector{
  // 2D
  float x;
  float y;

  Vector() : x(0), y(0) {}

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

  template <typename T>
  Vector operator*(T n) const{
    return {this->x * n, 
            this->y * n};
  }

  template <typename T>
  void operator*=(T n){
    this->x *= n; 
    this->y *= n;
  }

  // Generate a random vector based on bounds [a, b]
  void rand(float a, float b);

  // Compute dot product
  float Dot(const Vector &o) const;

  // Normalize vector
  void Normalize();

  // Generate a random normalized unit vector
  void randNorm();
};


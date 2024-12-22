#include "../include/vector.h"
#include <random>

void Vector::update(Vector u, float time){
  this->x += u.x * time;
  this->y += u.y * time;
}

void Vector::rand(float a, float b){
  // Random values
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<float> distrib(a, b);

  // Generate random values in between a and b
  this->x = distrib(gen);
  this->y = distrib(gen);
}

void Vector::randNorm(){
    // Random values
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(-1.0f, 1.0f);

    // generate random (non-zero) values
    float x, y;
    do{
      x = dist(gen);
      y = dist(gen);
    } while(x == 0.0f && y == 0.0f);

    float mag = std::sqrt(x*x + y*y);
    // Normalize vector
    this->x = x/mag;
    this->y = y/mag;

    // Scale vector to certain magnitude
    float scale_mag = 5000.0f; // Example magnitude
    this->x *= scale_mag;
    this->y *= scale_mag;
}


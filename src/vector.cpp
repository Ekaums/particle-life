#include "../include/vector.hpp"
#include <random>

void Vector::update(Vector &v1, float time){
  this->x += v1.x * time;
  this->y += v1.y * time;
}

void Vector::Normalize(){
  float mag = std::sqrt(x*x + y*y);

  // Normalize vector
  this->x = x/mag;
  this->y = y/mag;
}

float Vector::Dot(const Vector &v) const{
  return this->x * v.x + this->y * v.y;
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
}


#include "Arduino.h"
#include "Paddle.h"

Paddle::Paddle()
{
  this->L = 0;
  this->w = 0;
  this->h = 0;
  this->xAcc = 0;
  this->yAcc = 0;
  this->zAcc = 0;

  this->activated = false;
}

Paddle::Paddle(int L, int w, int h)
{
  this->L = L;
  this->w = w;
  this->h = h;
  this->xAcc = 0;
  this->yAcc = 0;
  this->zAcc = 0;

  this->activated = true;
}

int Paddle::getX() { return (int)this->x; }
int Paddle::getY() { return (int)this->y; }
int Paddle::getZ() { return (int)this->z; }

void Paddle::setActive(bool activated) { this->activated = activated; }
bool Paddle::isActive() { return this->activated; }


void Paddle::reset()
{
 this->x = this->L/2;
 this->y = this->w/2;
 this->z = this->h/2;
 }

void Paddle::go()
{
  this->x += this->xAcc;
  this->y += this->yAcc;
  this->z += this->zAcc;
}


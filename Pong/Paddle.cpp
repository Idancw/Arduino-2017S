#include "Arduino.h"
#include "Paddle.h"

Paddle::Paddle(int L, int w, int h)
{
  this->L = L;
  this->w = w;
  this->h = h;
  this->xAcc = 0;
  this->yAcc = 0;
  this->zAcc = 0;
}

int Paddle::getX() { return (int)this->x; }
int Paddle::getY() { return (int)this->y; }
int Paddle::getZ() { return (int)this->z; }

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


#include "Arduino.h"
#include "Paddle.h"

double FRICTION = 0.9;

Paddle::Paddle()
{
  this->L = 0;
  this->w = 0;
  this->h = 0;
  this->x = 0;
  this->y = 0;
  this->xVel = 0;
  this->yVel = 0;
  this->xAcc = 0;
  this->yAcc = 0;

  this->activated = false;
}

Paddle::Paddle(int L, int w, int h)
{
  this->L = L;
  this->w = w;
  this->h = h;
  this->x = 0;
  this->y = 0;
  this->xVel = 0;
  this->yVel = 0;
  this->xAcc = 0;
  this->yAcc = 0;

  this->activated = true;
}

double Paddle::getX() { return (int)this->x; }
double Paddle::getY() { return (int)this->y; }

void Paddle::setActive(bool activated) { this->activated = activated; }
bool Paddle::isActive() { return this->activated; }


void Paddle::reset()
{
  this->x = this->L/2;
  this->y = this->w/2;
  this->xVel = 0;
  this->yVel = 0;
  this->xAcc = 0;
  this->yAcc = 0;
}

void Paddle::go()
{
  if (xAcc > 0)
    this->xVel += 0.1;
  else if (xAcc < 0)
    this->xVel -= 0.1;
  else
    this->xVel *= FRICTION;

  if (yAcc > 0)
    this->yVel += 0.1;
  else if (yAcc < 0)
    this->yVel -= 0.1;
  else
    this->yVel *= FRICTION;
  
  this->x += this->xVel;
  this->y += this->yVel;
}

void Paddle::movingL() { this->xAcc = -1; }
void Paddle::movingR() { this->xAcc = 1; }
void Paddle::movingU() { this->yAcc = -1; }
void Paddle::movingD() { this->xAcc = 1; }

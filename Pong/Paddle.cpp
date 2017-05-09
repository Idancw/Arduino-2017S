#include "Arduino.h"
#include "Paddle.h"

double FRICTION = 0.9;
double MIN_PLEN = 1;
double MIN_PWID = 1;

Paddle::Paddle()
{
  this->activated = false;
}

Paddle::Paddle(int pLen, int pWid, int bLen, int bWid)
{
  this->pLen = pLen;
  this->pWid = pWid;
  this->bLen = bLen;
  this->bWid = bWid;
  reset();
  
  this->activated = true;
}

double Paddle::getX() { return (int)this->x; }
double Paddle::getY() { return (int)this->y; }

void Paddle::setActive(bool activated) { this->activated = activated; }
bool Paddle::isActive() { return this->activated; }


void Paddle::reset()
{
  this->x = this->bLen/2 - this->pLen/2;
  this->y = this->bWid/2 - this->pWid/2;
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

  // Keep velocities within [-1,1];
  this->xVel = max(this->xVel,-1);
  this->xVel = min(this->xVel,1);
  this->yVel = max(this->yVel,-1);
  this->yVel = min(this->yVel,1);

  // Move board according to velocity.
  this->x += this->xVel;
  this->y += this->yVel;

  // Keep board within the field
  this->x = max(this->x,0);
  this->x = min(this->x, this->bLen - this->pLen);
  this->y = max(this->y,0);
  this->y = min(this->y, this->bWid - this->pWid);
}

void Paddle::movingL() { this->xAcc = -1; }
void Paddle::movingR() { this->xAcc = 1; }
void Paddle::movingU() { this->yAcc = -1; }
void Paddle::movingD() { this->xAcc = 1; }

void Paddle::shrink(double factor)
{
  this->pLen *= factor;
  this->pWid *= factor;

  // Don't let the paddle get too small
  this->pLen = max(this->pLen,MIN_PLEN);
  this->pWid = min(this->pWid,MIN_PWID);

  // TODO: Consider recentering the paddle after it shrank.
}

bool Paddle::isBlocking(int a, int b)
{
  // TODO: Round to nearest lightbulb?
  return ((a > this->x) && (a < this->x + this->pLen) &&
            (b > this->y) && (b < this->y + this->pWid));
}






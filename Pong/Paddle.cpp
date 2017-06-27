#include "Arduino.h"
#include "Paddle.h"


// == GAME CONSTANTS ==
double FRICTION = 0.9;
double MIN_PLEN = 1;
double MIN_PWID = 1;

Paddle::Paddle(int pLen, int pWid, int bLen, int bWid)
{
  this->pLen = pLen-1;
  this->pWid = pWid-1;
  this->bLen = bLen-1;
  this->bWid = bWid-1;
  reset();
  
  this->activated = false;
}

void Paddle::setActive(bool new_activated) { this->activated = new_activated; }
bool Paddle::isActive() { return this->activated; }

void Paddle::reset()
{
  this->x = this->bLen/2 - this->pLen/2;
  this->y = this->bWid/2 - this->pWid/2;
}

void Paddle::go(int target_x, int target_y)
{
  // Keep board within the field
  this->x = max(target_x,0);
  this->x = min(this->x, this->bLen - this->pLen);
  this->y = max(target_y,0);
  this->y = min(this->y, this->bWid - this->pWid);
}


void Paddle::shrink(double factor)
{
  this->pLen *= factor;
  this->pWid *= factor;

  // Don't let the paddle get too small
  this->pLen = max(this->pLen,MIN_PLEN);
  this->pWid = min(this->pWid,MIN_PWID);

  // TODO: Consider recentering the paddle after it shrank.
}

bool Paddle::isBlocking(double a, double b, double r)
{
  // Use +r for anywhere on the ball, and assume ball is square.
  return ((a+r > this->x) && (a-r < this->x + this->pLen) &&
            (b+r > this->y) && (b-r < this->y + this->pWid));
}

String Paddle::getStr()
{
  return ("(" + String(x) + "-" + String(x+pLen) + ", " + y + "-" + String(y+pWid) + ")");
}





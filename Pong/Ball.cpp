#include "Arduino.h"
#include "Ball.h"

const int DEBUG = 0;

// == GAME CONSTANTS ==
int STEPS_PER_LED = 20;    // The amount of times Ball.go() must be called for the ball to go from one edge of the pixel to the other.
double MAXF = 4;    // The max factor the ball can be sped up by from the original speed.


Ball::Ball(double r, int L, int w, int h)
{
  this->spl = STEPS_PER_LED;
  this->L = L;
  this->w = w;
  this->h = h;
  setRadius(r);
  reset();
}

String Ball::getStr()
{
  return ("(" + String(this->x) + "," + this->y + "," + this->z + ")\t" +
          "(" + this->xVel + "," + this->yVel + "," + this->zVel + ")");
}


void Ball::setRadius(double r) { this->r = r; }

void Ball::reset()
{
  this->x = (this->L-1)/2.0;
  this->y = (this->w-1)/2.0;
  this->z = (this->h-1)/2.0;

  // Use seed from reading Voltage from unused pin.
  randomSeed(analogRead(0));
  
  // Give it an initial direction
//  this->xVel = 0.1;
//  this->yVel = 0.0;
//  this->zVel = 0.0;

  this->xVel = ((random(1,101)) * (random(2)*2-1)) / 100.0; // Get a number between (-1,1)/0
  this->yVel = ((random(1,101)) * (random(2)*2-1)) / 100.0; // Get a number between (-1,1)/0
  this->zVel = ((random(1,101)) * (random(2)*2-1)) / 100.0; // Get a number between (-1,1)/0

  this->normalizeVel();
  this->f = 1;
}

void Ball::normalizeVel()
{
  // Normalize
  double magnitude = sqrt(pow(this->xVel,2) + pow(this->yVel,2) + pow(this-> zVel,2));
//  double magnitude = sqrt(pow(this->xVel,2) + pow(this->yVel,2));
  this->xVel /= magnitude * spl;
  this->yVel /= magnitude * spl;
  this->zVel /= magnitude * spl;
}

void Ball::go()
{
  this->x += this->xVel;
  this->y += this->yVel;
  this->z += this->zVel;
  
  this->x = min(max(x,r),L-r);
  this->y = min(max(y,r),w-r);
  this->z = min(max(z,r),h-r);
}

void Ball::speedUp(double factor) // factor should be greater than 1
{
  if (this->f > MAXF)
    return;
  
  this->f *= factor; 
  this->xVel *= factor;
  this->yVel *= factor;
  this->zVel *= factor;
}

int Ball::checkBounce(Paddle &p1, Paddle &p2, Paddle &p3, Paddle &p4)
{
  double xv=this->xVel, yv=this->yVel, zv=this->zVel; 
  // If hits the floor, reverse
  if (this->z - this->r <= 0)
  {
    if (DEBUG)
      Serial.println("Hit floor!\t" + getStr());
    this->zVel *= -1;
  }
  // or ceiling
  else if (this->z + this->r >= h-1)
  {
    if (DEBUG)
      Serial.println("Hit ceiling!\t" + getStr());
  	this->zVel *= -1;
  }
  
  // Check for paddle blocks and gameovers
  if (this->x - this->r <= p1.isActive())
  {
    this->xVel *= -1;
    if (p1.isBlockingEdgeL(y,z,r))
    {
      xVel /= 2;
      normalizeVel();
    }
    if (p1.isBlockingEdgeU(y,z,r))
    {
      zVel /= 2;
      normalizeVel();
    }
    if (!p1.isBlocking(this->y, this->z, r))
      return 1;
    if (DEBUG)
      Serial.println("Hit p1!\t\t" + getStr());
  }
  if (this->x + this->r >= L-1-p2.isActive())
  {
    this->xVel *= -1;
    if (p2.isBlockingEdgeL(w-y,z,r))
      xVel /= 2;
    normalizeVel();
    if (p2.isBlockingEdgeU(w-y,z,r))
    {
      zVel /= 2;
      normalizeVel();
    }
    if (!p2.isBlocking(w-this->y, this->z, r))
      return 2;
    if (DEBUG)
      Serial.println("Hit p2!\t\t" + getStr());
  }
  if (this->y - this->r <= p3.isActive())
  {
    this->yVel *= -1;
    if (p3.isBlockingEdgeL(L-x,z,r))
    {
      yVel /= 2;
      normalizeVel();
    }
    if (p3.isBlockingEdgeU(L-x,z,r))
    {
      zVel /= 2;
      normalizeVel();
    }
    if (!p3.isBlocking(L-this->x, this->z, r))
      return 3;
    if (DEBUG)
      Serial.println("Hit p3!\t\t" + getStr() + p3.isActive());
  }
  if (this->y + this->r >= w-1-p4.isActive())
  {
    this->yVel *= -1;
    if (p4.isBlockingEdgeL(x,z,r))
    {
      xVel /= 2;
      normalizeVel();
    }
    if (p4.isBlockingEdgeD(x,z,r))
    {
      zVel /= 2;
      normalizeVel();
    }
    if (!p4.isBlocking(this->x, this->z, r))
      return 4;
    if (DEBUG)
      Serial.println("Hit p4!\t\t" + getStr());
  }  
  return 0;
}





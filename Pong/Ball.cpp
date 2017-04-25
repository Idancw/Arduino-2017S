#include "Arduino.h"
#include "Ball.h"

Ball::Ball(int r, int L, int w, int h)
{  
  this->L = L;
  this->w = w;
  this->h = h;
  setRadius(r);
  reset();
}

void Ball::setRadius(int r) { this->r = r; }

int Ball::getX() { return (int)this->x; }
int Ball::getY() { return (int)this->y; }
int Ball::getZ() { return (int)this->z; }
char[] Ball:toStr() { return "

void Ball::reset()
{
 this->x = this->L/2;
 this->y = this->w/2;
 this->z = this->h/2;
 
 // Give it a random direction
 this->xVel = ((random(1,101)) * (random(2)*2-1)) / 100.0; // Get a number between (-1,1)/0
 this->yVel = ((random(1,101)) * (random(2)*2-1)) / 100.0; // Get a number between (-1,1)/0
 this->zVel = ((random(1,101)) * (random(2)*2-1)) / 100.0; // Get a number between (-1,1)/0

 // Normalize
 double magnitude = sqrt(pow(this->xVel,2) + pow(this->yVel,2) + pow(this-> zVel,2));
 this->xVel /= magnitude;
 this->yVel /= magnitude;
 this->zVel /= magnitude;
}

void Ball::go()
{
  this->x += this->xVel;
  this->y += this->yVel;
  this->z += this->zVel;
  checkBounce();
}

void Ball::speedUp(int factor) // factor should be greater than 1
{
  this->xVel *= factor;
  this->yVel *= factor;
  this->zVel *= factor;
}

void Ball::checkBounce()
{
  // If hits the ceiling or floor, reverse
  if (this->z - this->r <= 0 || this->z + this->r >= h)
  	this->zVel *= -1;
  
  // TODO: 0 if no paddle. use 1 when there is and it hits.
  if (this->x - this->r <= 0)	// Hits p1
  	this->xVel *= -1;
  if (this->x + this->r >= L)	// Hits p2
  	this->xVel *= -1;
  if (this->y - this->r <= 0)	// Hits p3
  	this->yVel *= -1;
  if (this->y + this->r >= L)	// Hits p4
  	this->yVel *= -1;

  // TODO: If it hits a wall, we'll need to rotate the vector 
  //	- https://www.youtube.com/watch?v=DOMg0lXWatM
  //	- https://bitbucket.org/MegaJiXiang/arduinoprojects/src/9bdaad4ad04252ad85d862f9f0945c1f82a46da8/Libraries/Math/
  // rotateBy(deg);
}

void rotateBy(int theta)
{
	// Do something.
}




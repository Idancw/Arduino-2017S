#include "Arduino.h"
#include "Ball.h"

const int DEBUG = 0;

// == GAME CONSTANTS ==
double SPEEDUP = 1.1;   // Multiplier to speed the ball up by.
double STEPS_PER_LED = 30;    // The amount of times Ball.go() must be called for the ball to go from one edge of the pixel to the other.
double MAXF = 3.5;    // The max factor the ball can be sped up by from the original speed.

double EDGE_MULT = 1;   // The amount an Edge Hit pushes the ball in that direction.

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
          "(" + this->xVel*1000 + "," + this->yVel*1000 + "," + this->zVel*1000 + ")");
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

  this->xVel = ((random(25,51)) * (random(2)*2-1)); // Get a number between (-1,1)/0
  this->yVel = ((random(25,51)) * (random(2)*2-1)); // Get a number between (-1,1)/0
  this->zVel = ((random(25,max(abs(xVel), abs(yVel)))) * (random(2)*2-1)); // Get a number between (-1,1)/0

  xVel /= 50.0;
  yVel /= 50.0;
  zVel /= 50.0;

  this->f = 1;

  this->normalizeVel();
  speedup = SPEEDUP;
}

void Ball::normalizeVel()
{
  // Normalize
  double magnitude = sqrt(pow(this->xVel,2) + pow(this->yVel,2) + pow(this-> zVel,2));
  this->xVel /= magnitude * spl * f;
  this->yVel /= magnitude * spl * f;
  this->zVel /= magnitude * spl * f;
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

void Ball::speedUp() // factor should be greater than 1
{
  if (this->f > MAXF)
    return;
    
  this->f *= speedup; 
  this->xVel *= speedup;
  this->yVel *= speedup;
  this->zVel *= speedup;

  speedup = 1 + (speedup-1)/speedup;
  Serial.println("Speedup = " + String(100*speedup));
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

  // Check if p1 was hit
  if (this->x - this->r <= p1.isActive())
  {
    if (p1.isActive() && !p1.isBlocking(this->y, this->z, r))
      return 1;

    this->xVel *= -1;
    go();
//    x = p1.isActive() + r;
    
//      
//    // Check if hit edge of paddle
//    if (p1.isBlockingEdgeL(y,z,r))
//    {
//      yVel = abs(yVel) * EDGE_MULT * -1;
//      normalizeVel();
//    }
//    else if (p1.isBlockingEdgeR(y,z,r))
//    {
//      yVel = abs(yVel) * EDGE_MULT * 1;
//      normalizeVel();
//    }
//    if (p1.isBlockingEdgeD(y,z,r))
//    {
//      zVel = abs(zVel) * EDGE_MULT * -1;
//      normalizeVel();
//    }
//    else if (p1.isBlockingEdgeU(y,z,r))
//    {
//      zVel = abs(zVel) * EDGE_MULT * 1;
//      normalizeVel();
//    }

    if (DEBUG)
      Serial.println("Hit p1!\t\t" + getStr());
  }

  // Check if p2 was hit
  else if (this->x + this->r >= L-1-p2.isActive())
  {
    if (p2.isActive() && !p2.isBlocking(w-1-this->y, this->z, r))
      return 2;

    this->xVel *= -1;
    go();
//    x = L-1-p2.isActive() - r;

//    // Check if hit edge of paddle
//    if (p2.isBlockingEdgeL(w-1-y,z,r))
//    {
//      yVel = abs(yVel) * EDGE_MULT * 1;
//      normalizeVel();
//    }
//    else if (p2.isBlockingEdgeR(w-1-y,z,r))
//    {
//      yVel = abs(yVel) * EDGE_MULT * -1;
//      normalizeVel();
//    }
//    if (p2.isBlockingEdgeD(w-1-y,z,r))
//    {
//      zVel = abs(zVel) * -1;
//      normalizeVel();
//    }
//    else if (p2.isBlockingEdgeR(w-1-y,z,r))
//    {
//      zVel = abs(zVel) * 1;
//      normalizeVel();
//    }

    if (DEBUG)
      Serial.println("Hit p2!\t\t" + getStr());
  }
  
  // Check if p3 was hit
  if (this->y - this->r <= p3.isActive())
  {
    if (p3.isActive() && !p3.isBlocking(L-1-this->x, this->z, r))
      return 3;
      
    this->yVel *= -1;
    go();
//    y = p3.isActive() + r;
    
//    // Check if hit edge of paddle
//    if (p3.isBlockingEdgeL(L-1-x,z,r))
//    {
//      xVel = abs(xVel) * EDGE_MULT * 1;
//      normalizeVel();
//    }
//    else if (p3.isBlockingEdgeR(L-1-x,z,r))
//    {
//      xVel = abs(xVel) * EDGE_MULT * -1;
//      normalizeVel();
//    }
//    if (p3.isBlockingEdgeD(L-1-x,z,r))
//    {
//      zVel = abs(zVel) * -1;
//      normalizeVel();
//    }
//    else if (p3.isBlockingEdgeR(L-1-x,z,r))
//    {
//      zVel = abs(zVel) * 1;
//      normalizeVel();
//    }    

    if (DEBUG)
      Serial.println("Hit p3!\t\t" + getStr() + p3.isActive());
  }

  // Check if p4 was hit
  if (this->y + this->r >= w-1-p4.isActive())
  {
    if (p4.isActive() && !p4.isBlocking(this->x, this->z, r))
      return 4;
    
    this->yVel *= -1;
    go();
//    y = w-1-p4.isActive() - r;
    
//    // Check if hit edge of paddle
//    if (p4.isBlockingEdgeL(x,z,r))
//    {
//      xVel = abs(xVel) * EDGE_MULT * -1;
//      normalizeVel();
//    }
//    else if (p4.isBlockingEdgeR(x,z,r))
//    {
//      xVel = abs(xVel) * EDGE_MULT * 1;
//      normalizeVel();
//    }
//    if (p4.isBlockingEdgeD(x,z,r))
//    {
//      zVel = abs(zVel) * 1;
//      normalizeVel();
//    }
//    else if (p4.isBlockingEdgeR(x,z,r))
//    {
//      zVel = abs(zVel) * 1;
//      normalizeVel();
//    }

    if (DEBUG)
      Serial.println("Hit p4!\t\t" + getStr());
  }  
  return 0;
}





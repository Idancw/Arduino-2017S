#include "Arduino.h"
#include "Ball.h"


// == GAME CONSTANTS ==
int STEPS_PER_LED = 50;    // The amount of times Ball.go() must be called for the ball to go from one edge of the pixel to the other.
double MAXF = 8;


Ball::Ball(double r, int L, int w, int h)
{
  this->spl = STEPS_PER_LED;
  this->L = L;
  this->w = w;
  this->h = h;
  setRadius(r);
  reset();
}

float Ball::getR() { return this->r; }
float Ball::getX() { return this->x; }
float Ball::getY() { return this->y; }
float Ball::getZ() { return this->z; }

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
  // Give it a random direction
  this->xVel = ((random(1,101)) * (random(2)*2-1)) / 100.0; // Get a number between (-1,1)/0
  this->yVel = ((random(1,101)) * (random(2)*2-1)) / 100.0; // Get a number between (-1,1)/0
  this->zVel = ((random(1,101)) * (random(2)*2-1)) / 100.0; // Get a number between (-1,1)/0
  
  // Normalize
//  double magnitude = sqrt(pow(this->xVel,2) + pow(this->yVel,2) + pow(this-> zVel,2));
  double magnitude = sqrt(pow(this->xVel,2) + pow(this->yVel,2));
  this->xVel /= magnitude * spl;
  this->yVel /= magnitude * spl;
//  this->zVel /= magnitude * spl;

  this->f = 1;
}

void Ball::go()
{
  this->x += this->xVel;
  this->y += this->yVel;
  this->z += this->zVel;
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

//void Ball::checkBounce()
int Ball::checkBounce(Paddle &p1, Paddle &p2, Paddle &p3, Paddle &p4)
{
  double xv=this->xVel, yv=this->yVel, zv=this->zVel; 
  // If hits the floor, reverse
  if (this->z - this->r <= 0)
  {
    this->zVel *= -1;
    bresenham_line_3d(8.0/this->zVel);
  }
  // or ceiling
  else if (this->z + this->r >= h)
  {
  	this->zVel *= -1;
    bresenham_line_3d(-8.0/this->zVel);
  }

  // Check for paddle block or gameover
  if (p1.isActive() && this->x - this->r < 1)
    if (p1.isBlocking(this->y, this->z))
    {
      this->xVel *= -1;    // TODO: Not just 180, but by something based on location on paddle
    }
    else
      return 1;
  // ...for p2-p4

  
  // TODO: 0 if no paddle. use 1 when there is and it hits.
  if (this->x - this->r <= 0)	// Hits p1
  {
  	this->xVel *= -1;
    bresenham_line_3d(8.0/this->xVel);
  }
  if (this->x + this->r >= L-1)	// Hits p2
  {
  	this->xVel *= -1;
    bresenham_line_3d(-8.0/this->xVel);
  }
  if (this->y - this->r <= 0)	// Hits p3
  {
  	this->yVel *= -1;
    bresenham_line_3d(8.0/this->yVel);
  }
  if (this->y + this->r >= w-1)	// Hits p4
  {
  	this->yVel *= -1;
    bresenham_line_3d(-8.0/this->yVel);
  }

  // TODO: If it hits a paddle, we'll need to rotate the vector by some amount.
  //rotateBy(CalculateRotationAngel);

  // If redirected, recalculate 
  if (xv!=this->xVel || yv!=this->yVel || zv!=this->zVel)
  {
//    bresenham_line_3d();
  }
  
  return 0;
}

void rotateBy(double theta)
{
	// Rotate the ball's direction by theta degrees counterclockwise.
  //  - https://www.youtube.com/watch?v=DOMg0lXWatM
  //  - https://bitbucket.org/MegaJiXiang/arduinoprojects/src/9bdaad4ad04252ad85d862f9f0945c1f82a46da8/Libraries/Math/

}

void Ball::bresenham_line_3d(double mult)
{
    double x0=this->x, y0=this->y, z0=this->z;
    double x1=x0*mult, y1=y0*mult, z1=z0*mult;
    double dx = abs(x1 - x0);
    double dy = abs(y1 - y0);
    double dz = abs(z1 - z0);
    int x=x0, y=y0, z=z0;
    int sx = (x0 > x1)? -1 : 1;
    int sy = (y0 > y1)? -1 : 1;
    int sz = (z0 > z1)? -1 : 1;

    if (dz > dx && dz > dy)   // Strongest going up/down (z)
    {
        double err_x = dz / 2.0;
        double err_y = dz / 2.0;
        while (z != z1)
        {
            this->points[int(z)][0] = x;
            this->points[int(z)][1] = y;
            this->points[int(z)][2] = z;
            err_x -= dx;
            if (err_x < 0)
            {
                x += sx;
                err_x += dz;
            }
            err_y -= dy;
            if (err_y < 0)
            {
                y += sy;
                err_y += dz;
            }
            z += sz;
        }
    }
    else if (dx > dy)   // Strongest going left/right (x)
    {
        int err_z = dx / 2.0;
        int err_y = dx / 2.0;
        while (x != x1)
        {
            this->points[int(x)][0] = x;
            this->points[int(x)][1] = y;
            this->points[int(x)][2] = z;
            err_y -= dy;
            if (err_y < 0)
            {
                y += sy;
                err_y += dx;
            }
            err_z -= dz;
            if (err_z < 0)
            {
                z += sz;
                err_z += dx;
            }
            x += sx;
        }
    }
    else    // Strongest going back/front (y)
    {
        int err_x = dy / 2.0;
        int err_z = dy / 2.0;
        while (y != y1)
        {
            this->points[int(y)][0] = x;
            this->points[int(y)][1] = y;
            this->points[int(y)][2] = z;
            err_x -= dx;
            if (err_x < 0)
            {
                x += sx;
                err_x += dy;
            }
            err_z -= dz;
            if (err_z < 0)
            {
                z += sz;
                err_z +=  dy;
            }
            y += sy;
        }
    }
    this->points[7][0] = x1;
    this->points[7][1] = y1;
    this->points[7][2] = z1;
    return;
}


#ifndef Ball_h
#define Ball_h

#include "Arduino.h"
#include "Paddle.h"


class Ball
{
  private:
    int L, w, h;
    int spl;   // steps per LED  - how many go()'s it takes for the ball to travel across 1 LED
   
  public:
    double points[8][3];   // Bresenham's Line algorithm in 3D results
    double r, x, xVel, y, yVel, z, zVel, speedup, f;
    Ball(double r, int L, int w, int h);
    void setRadius(double r);
    String getStr();
    void reset();
    void normalizeVel();
    void go();
    void speedUp();
    int checkBounce(Paddle &p1, Paddle &p2, Paddle &p3, Paddle &p4);
};

#endif

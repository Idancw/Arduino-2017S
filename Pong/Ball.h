#ifndef Ball_h
#define Ball_h

#include "Arduino.h"
#include "Paddle.h"


class Ball
{
  private:
    double r, x, xVel, y, yVel, z, zVel;
    int L, w, h;
    int spl;   // steps per LED  - how many go()'s it takes for the ball to travel across 1 LED
   
  public:
    Ball(int r, int L, int w, int h);
    void setRadius(int r);
    int getX();
    int getY();
    int getZ();
    void reset();
    void go();
    void speedUp(int factor);
//    void checkBounce();
    int checkBounce(Paddle &p1, Paddle &p2, Paddle &p3, Paddle &p4);
    void rotateBy(int theta);
    // TODO: Maybe some draw function that cares about the radius of the ball and returns the range of coords
};

#endif

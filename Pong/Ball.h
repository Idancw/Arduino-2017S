#ifndef Ball_h
#define Ball_h

#include "Arduino.h"

class Ball
{
  private:
    double r, x, xVel, y, yVel, z, zVel;
    int L, w, h;
   
  public:
    Ball(int r, int L, int w, int h);
    void setRadius(int r);
    int getX();
    int getY();
    int getZ();
    void reset();
    void go();
    void speedUp(int factor);
    void checkBounce();
    void rotateBy(int theta);
    // TODO: Maybe some draw function that cares about the radius of the ball and returns the range of coords
};

#endif

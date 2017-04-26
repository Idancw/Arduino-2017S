#ifndef Paddle_h
#define Paddle_h

#include "Arduino.h"

class Paddle
{
  private:
    double x, xAcc, y, yAcc, z, zAcc;
    int L, w, h;
    bool activated;
   
  public:
    Paddle();
    Paddle(int L, int w, int h);
    int getX();
    int getY();
    int getZ();
    // int getL();
    // int getW();
    // int getH();
    void setActive(bool active);
    bool isActive();
    void reset();
    void go();
    // TODO: Maybe some draw function that cares about the radius of the ball and returns the range of coords
};

#endif

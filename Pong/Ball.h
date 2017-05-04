#ifndef Ball_h
#define Ball_h

#include "Arduino.h"
#include "Paddle.h"


class Ball
{
  private:
    double r, x, xVel, y, yVel, z, zVel, f;
    int L, w, h;
    int spl;   // steps per LED  - how many go()'s it takes for the ball to travel across 1 LED
   
  public:
    Ball(double r, int L, int w, int h);
    void setRadius(double r);
    String getStr();
    float getR();
    float getX();
    float getY();
    float getZ();
    void reset();
    void go();
    void speedUp(double factor);
//    void checkBounce();
    int checkBounce(Paddle &p1, Paddle &p2, Paddle &p3, Paddle &p4);
    void rotateBy(double theta);
    // TODO: Maybe some draw function that cares about the radius of the ball and returns the range of coords
};

#endif

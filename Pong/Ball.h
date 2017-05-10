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
    double r, x, xVel, y, yVel, z, zVel, f;
    Ball(double r, int L, int w, int h);
    void setRadius(double r);
    String getStr();
    void reset();
    void go();
    void speedUp(double factor);
//    void checkBounce();
    int checkBounce(Paddle &p1, Paddle &p2, Paddle &p3, Paddle &p4);
    void rotateBy(double theta);
    void bresenham_line_3d(double mult);//double x1, double y1, double z1);
    // TODO: Maybe some draw function that cares about the radius of the ball and returns the range of coords
};

#endif

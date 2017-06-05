#ifndef Paddle_h
#define Paddle_h

#include "Arduino.h"

class Paddle
{
//  private:
  public:
    double x, xVel, xAcc, y, yVel, yAcc;
    int pLen, pWid, bLen, bWid;
    bool activated;
   
    Paddle();
    Paddle(int pLen, int pWid, int bLen, int bWid);
    double getX();
    double getY();
    void movingL();
    void movingR();
    void movingU();
    void movingD();
    // int getL();
    // int getW();
    // int getH();
    void setActive(bool active);
    bool isActive();
    void reset();
    void go();
    void shrink(double factor);
    bool isBlocking(int a, int b);
    // TODO: Maybe some draw function that cares about the radius of the ball and returns the range of coords
};

#endif

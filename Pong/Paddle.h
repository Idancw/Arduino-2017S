#ifndef Paddle_h
#define Paddle_h

#include "Arduino.h"

class Paddle
{
//  private:
  public:
    int x, y;
    int pLen, pWid, bLen, bWid;
    bool activated;
   
    Paddle();
    Paddle(int pLen, int pWid, int bLen, int bWid);
    void setActive(bool active);
    bool isActive();
    void reset();
//    void setTarget(int target_x, int target_y);
    void go(int target_x, int target_y);
    void shrink(double factor);
    bool isBlocking(double a, double b, double r);
    String getStr();
    // TODO: Maybe some draw function that cares about the radius of the ball and returns the range of coords
};

#endif

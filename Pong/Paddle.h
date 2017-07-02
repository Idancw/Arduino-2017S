#ifndef Paddle_h
#define Paddle_h

#include "Arduino.h"

class Paddle
{
//  private:
  public:
    int x, y;
    int pLen, pWid, bLen, bWid;
    int activated;
   
    Paddle(int pLen, int pWid, int bLen, int bWid);
    void setActive(bool new_activated);
    int isActive();
    void reset();
//    void setTarget(int target_x, int target_y);
    void go(int target_x, int target_y);
    void shrink(double factor);
    bool isBlocking(double a, double b, double r);
    bool isOverlapping(double a, double b, double c, double d);
    bool isBlockingEdgeL(double a, double b, double r);
    bool isBlockingEdgeR(double a, double b, double r);
    bool isBlockingEdgeD(double a, double b, double r);
    bool isBlockingEdgeU(double a, double b, double r);
    String getStr();
    // TODO: Maybe some draw function that cares about the radius of the ball and returns the range of coords
};

#endif

#include "Ball.h"
#include "Paddle.h"

Ball Ball(1,8,8,1);
int t = 0;


void setup()
{
  randomSeed(analogRead(A0)); // uses seed from reading Voltage from unused pin.
}


void loop()
{
  Ball.go();
  drawBoard();


  // Every second, increase speed by 1%
  if (t % 1000 == 0)
    Ball.speedUp(1.01);
   
  t++;
  delay(1);
}

void drawBoard()
{
  // Light up board according to ball and paddles.
}



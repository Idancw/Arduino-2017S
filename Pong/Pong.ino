#include "Ball.h"
#include "Paddle.h"

Ball ball(1,8,8,1);
int t = 0;
int gamestatus = -1;  // 0 = playing
                      // [1,4] = that player has just lost.
                      // -1 = paused
Paddle p1 = Paddle();
Paddle p2 = Paddle();
Paddle p3 = Paddle();
Paddle p4 = Paddle();

// Settings:
bool speedUpBall = true;
bool shrinkPaddles = true;


void setup()
{
  randomSeed(analogRead(A0)); // uses seed from reading Voltage from unused pin.
}


void loop()
{
  drawBoard();
  Serial.println('(' + ball.getX() + ',' + ball.getY() + ',' + ball.getZ() + ')');

  // TODO: Get commands from input devices
  
  if (gamestatus == 1)
  {    
    ball.go();
    gamestatus = ball.checkBounce(p1, p2, p3, p4);

    // Every second, increase speed by 1%
    if (t % 1000 == 0)
    {
      ball.speedUp(1.01);
      p1.shrink(0.99);
      p2.shrink(0.99);
      p3.shrink(0.99);
      p4.shrink(0.99);
    } 
    
  }
  else if (gamestatus > 0)
  {
    t = 0;
    flashLosingEdge(gamestatus, t);
  }
  t++;
  delay(1);
}

void drawBoard()
{
  // Light up board according to ball and paddles.
}

void flashLosingEdge(int player, int t)
{
  switch (player)
  {
    case 1:
      // TODO: Define board for player 1
    case 2:
    case 3:
    case 4:
    default:
    break;
  }
  if (t % 500 == 0)
  {
    Serial.println("Player " + String(player) + " has lost");
    // TODO: Invert lights on edge to flash pattern
  }
}



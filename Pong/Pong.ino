#include "Ball.h"
#include "Paddle.h"

// DEBUGGING
const int DEBUG = 0;
const int USE_OLD_LIGHTING = 0;


// == GAME CONSTANTS ==
double SPEEDUP = 1.005;


byte PIN_CLOCKS[8] = {15,14,13,12,11,10,9,8};

// pin connected to DS of 74HC595
byte PIN_DATA_OUT[8];

int t = 0;
int gamestatus = -1;  // 0 = playing
                      // [1,4] = that player has just lost.
                      // -1 = paused

int L=8,W=8,H=8;
Ball ball(0.5,L,W,H);
Paddle p1 = Paddle();
Paddle p2 = Paddle();
Paddle p3 = Paddle();
Paddle p4 = Paddle();

int board[8][8][8];


// Settings:
bool speedUpBall = true;
bool shrinkPaddles = true;

void setup()
{
  for (int i=0; i < 8; i++)
  {
    PIN_DATA_OUT[i] = i;
    pinMode(PIN_DATA_OUT[i], OUTPUT);
    pinMode(PIN_CLOCKS[i], OUTPUT);
  }
  
//  Serial.begin(9600); //set baud rate

  gamestatus = 0;
  ball.reset();
}


void loop()
{
  //  Serial.println(t);
  if (t%1==0)
    Serial.println(ball.getStr());
  
  drawBoard();


  // TODO: Get commands from input devices
  
  if (gamestatus == 0)
  {    
    ball.go();
    gamestatus = ball.checkBounce(p1, p2, p3, p4);

    // Every second, increase speed by 1%
    if (t % 100 == 0)
    {
      if (speedUpBall)
        ball.speedUp(SPEEDUP);

      if (shrinkPaddles)
      {
        p1.shrink(0.999);
        p2.shrink(0.999);
        p3.shrink(0.999);
        p4.shrink(0.999);
      }
    }
  }
  
  else if (gamestatus > 0)
  {
    t = 0;
    flashLosingEdge(gamestatus, t);
  }
  t++;
  delay(1);
  
  if (DEBUG)
    delay(499);
}


void drawBoard()
{
  // Clear board
  for (int i=0; i<8; i++)
    for (int j=0; j<8; j++)
      for (int k=0; k<8; k++)
        board[i][j][k] = 0;//(i+j+k)%2;

//Debug Mode
if (DEBUG)
  if (t%2)
    for (int i=0; i<8; i++)
      for (int j=0; j<8; j++)
        for (int k=0; k<8; k++)
          board[i][j][k] = (i+j+k)%2;
  else
    for (int i=0; i<8; i++)
      for (int j=0; j<8; j++)
        for (int k=0; k<8; k++)
          board[i][j][k] = 1-(i+j+k)%2;


  float r = ball.r;
  float x = ball.x;
  float y = ball.y;
  float z = ball.z;

  Serial.println("Ball's dim are:\t" + String(round(x-r)) + "-" + round(x+r) + "\t" +
                  round(y-r) + "-" + round(y+r) + "\t" +
                  round(z-r) + "-" + round(z+r));

// OLD LIGHTING METHOD
  if (USE_OLD_LIGHTING)
  {
    // Light pixels that are +-0.5 from any piece of the ball.
    for (int i=max(0,int(x-r+0.5)); i<=min(7,int(x+r+0.5)); i++)
      for (int j=max(0,int(y-r+0.5)); j<=min(7,int(y+r+0.5)); j++)
        for (int k=0; k<min(8,8); k++)
          board[i][j][k] = 1;
    showBoard();
    return;
  }
  
  int points_i = 0;
  if (ball.xVel > ball.yVel && ball.xVel > ball.zVel)
    points_i = ball.x;
  else if (ball.yVel > ball.zVel)
    points_i = ball.y;
  else 
    points_i = ball.z;

  int i = ball.points[points_i][0];
  int j = ball.points[points_i][1];
  int k = ball.points[points_i][2];
  board[i][j][k] = 1;
  showBoard();
}


// Actually show board
void showBoard()
{
  int k = 0;
  for (int i=0; i<8; i++)
  {
    // Load data for the i-th row
    for (int j=0; j<8; j++)
    {      
        if (board[i][j][k] == 1)
          digitalWrite(PIN_DATA_OUT[j], HIGH);
        else
          digitalWrite(PIN_DATA_OUT[j], LOW);
    }

    digitalWrite(PIN_CLOCKS[i], HIGH);
    digitalWrite(PIN_CLOCKS[i], LOW);
  }
}

void flashLosingEdge(int player, int t)
{
  switch (player)
  {
    case 1:
      if (t%500)
        for (int i=0; i<8; i++)
          board[0][i][0] = i%2;
      else if (t%1000)
        for (int i=0; i<8; i++)
          board[0][i][0] = 1-(i%2);
      break;
    case 2:
      if (t%500)
        for (int i=0; i<8; i++)
          board[8][i][0] = i%2;
      else if (t%1000)
        for (int i=0; i<8; i++)
          board[8][i][0] = 1-(i%2);
      break;
    case 3:
      if (t%500)
        for (int i=0; i<8; i++)
          board[i][0][0] = i%2;
      else if (t%1000)
        for (int i=0; i<8; i++)
          board[i][0][0] = 1-(i%2);
      break;
    case 4:
      if (t%500)
        for (int i=0; i<8; i++)
          board[i][8][0] = i%2;
      else if (t%1000)
        for (int i=0; i<8; i++)
          board[i][8][0] = 1-(i%2);
      break;
    default:
    break;
  }
  if (t % 500 == 0)
  {
    Serial.println("Player " + String(player) + " has lost");
  }
}



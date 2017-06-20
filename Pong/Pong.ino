#include "Arduino.h"
#include "Ball.h"
#include "Paddle.h"

// DEBUGGING
const int DEBUG = 0;    // 1 = flash every other light
const int USE_OLD_LIGHTING = 1;   // 1 = simple, zig-zagging method
const int LIGHT_ALL = 0;    // 1 = just light everything up all the time

// == GAME CONSTANTS ==
double SPEEDUP = 1.005;
int players = 4;



byte PIN_DATA_OUT[8] = {2,3,4,5,6,7,8,9};
byte PIN_CLOCKS[8] = {22,23,24,25,26,27,28,29};
byte PIN_TRANSISTORS[8] = {42,43,44,45,46,47,48,49};

int t = 0;
int gamestatus = -1;  // 0 = playing
                      // [1,4] = that player has just lost.
                      // -1 = paused

int L=8,W=8,H=8;
int pL=3,pW=3;
Ball ball(0.5,L,W,H);
Paddle p1 = (players > 0)? Paddle(pL,pW,L,H) : Paddle();    // West
Paddle p2 = (players > 1)? Paddle(pL,pW,L,H) : Paddle();    // East
Paddle p3 = (players > 2)? Paddle(pL,pW,L,H) : Paddle();    // North
Paddle p4 = (players > 3)? Paddle(pL,pW,L,H) : Paddle();    // South
Paddle ps[4] = {p1,p2,p3,p4};

int board[8][8][8];


// Settings:
bool speedUpBall = false;
bool shrinkPaddles = false;

void setup()
{
  for (int i=0; i < 8; i++)
  {
    pinMode(PIN_DATA_OUT[i], OUTPUT);
    pinMode(PIN_CLOCKS[i], OUTPUT);
    pinMode(PIN_TRANSISTORS[i], OUTPUT);
  }
  
  Serial.begin(9600); //set baud rate

  gamestatus = 0;
  ball.reset();
}


void loop()
{
//  Serial.println(t);
//  if (t%1==0)
//    Serial.println(ball.getStr());

  // TODO: Get commands from input devices
//  for (int i=0; i < 8; i++)
//  {
//    digitalWrite(PIN_CLOCKS[i], LOW);
//    digitalWrite(PIN_CLOCKS[i], HIGH);  
//  }
  
  if (gamestatus == 0)
  {
    drawBoard();
    showBoard();
    
    ball.go();
    p1.go(ball.y-pL/2, ball.z-pW/2);    // Follow the ball
    p2.go(L-ball.y-pL/2, ball.z-pW/2);    // Follow the ball
    p3.go(W-ball.x-pL/2, ball.z-pW/2);    // Follow the ball
    p4.go(ball.x-pL/2, ball.z-pW/2);    // Follow the ball
    gamestatus = ball.checkBounce(p1, p2, p3, p4);
    
    // Every second, increase speed by 1%
    if (t % 100 == 0)
    {
      if (speedUpBall)
        ball.speedUp(SPEEDUP);

      if (shrinkPaddles)
      {
        for (int i=0; i<4; i++)
          ps[i].shrink(0.999);
      }
    }
    if (gamestatus > 0)
      Serial.println("Player " + String(gamestatus) + " has lost");
  }
  else if (gamestatus > 0)
  {
    Serial.println("p" + String(gamestatus) + ": " + ps[gamestatus-1].getStr());    
//    Serial.println("t = " + String(t));
    printPaddles();
    printBall();
    flashLosingEdge(gamestatus, t);
    showBoard();
    delay(1);
  }
  t++;
//  delay(1);
  
//  if (DEBUG)
//    delay(499);
}


void drawBoard()
{
  // Clear board
  for (int i=0; i<8; i++)
    for (int j=0; j<8; j++)
      for (int k=0; k<8; k++)
        board[i][j][k] = 0;//(i+j+k)%2;
  
  board[0][0][0] = 1;   // For showing orientation

  //Debug Mode
  if (DEBUG)
  {
    if ((t%200)<100)
      for (int i=0; i<8; i++)
        for (int j=0; j<8; j++)
          for (int k=0; k<8; k++)
            board[i][j][k] = (i+j+k)%2;
    else
      for (int i=0; i<8; i++)
        for (int j=0; j<8; j++)
          for (int k=0; k<8; k++)
            board[i][j][k] = 1-(i+j+k)%2;
    return;
  }

  printPaddles();
  printBall();
}

void printPaddles()
{
  if (p1.activated)
    for (int i=max(0,p1.x); i<=min(7,p1.x+p1.pLen); i++)
      for (int j=max(0,p1.y); j<=min(7,p1.y+p1.pWid); j++)
          board[0][i][j] = 1;
          
  if (p2.activated)
    for (int i=max(0,int(p2.x+0.5)); i<=min(7,int(p2.x+p2.pLen+0.5)); i++)
      for (int j=max(0,int(p2.y+0.5)); j<=min(7,int(p2.y+p2.pWid+0.5)); j++)
          board[7][7-i][j] = 1;
          
  if (p3.activated)
    for (int i=max(0,int(p3.x+0.5)); i<=min(7,int(p3.x+p3.pLen+0.5)); i++)
      for (int j=max(0,int(p3.y+0.5)); j<=min(7,int(p3.y+p3.pWid+0.5)); j++)
          board[i][0][j] = 1;
          
  if (p4.activated)
    for (int i=max(0,int(p4.x+0.5)); i<=min(7,int(p4.x+p4.pLen+0.5)); i++)
      for (int j=max(0,int(p4.y+0.5)); j<=min(7,int(p4.y+p4.pWid+0.5)); j++)
          board[7-i][7][j] = 1;
}

void printBall()
{
  float r = ball.r;
  float x = ball.x;
  float y = ball.y;
  float z = ball.z;

//  Serial.println("Ball's dim are:\t" + String(round(x-r)) + "-" + round(x+r) + "\t" +
//                  round(y-r) + "-" + round(y+r) + "\t" +
//                  round(z-r) + "-" + round(z+r));

// OLD LIGHTING METHOD
  if (USE_OLD_LIGHTING)
  {
    // Light pixels that are +-0.5 from any piece of the ball.
    for (int i=max(0,int(x-r+0.5)); i<=min(7,int(x+r+0.5)); i++)
      for (int j=max(0,int(y-r+0.5)); j<=min(7,int(y+r+0.5)); j++)
        for (int k=max(0,int(z-r+0.5)); k<=min(7,int(z+r+0.5)); k++)
          board[i][j][k] = 1;
  }
  else // SMOOTH METHOD
  {
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
    board[i][j][0] = 1;
  }
  
  if (t%1000==0)
  {
    Serial.println("Ball: " + ball.getStr());
    Serial.println("P1's pos: " + p1.getStr());
    Serial.println("P2's pos: " + p2.getStr());
    Serial.println("P3's pos: " + p3.getStr());
    Serial.println("P4's pos: " + p4.getStr());
  }
}

void showBoard()
{
  for (int k=0; k<4; k++)
  {
    for (int i=0; i<8; i++)
    {
      // Load data for the i-th row
      for (int j=0; j<8; j++)
      {
        if (board[i][j][k] == 1 || LIGHT_ALL)
          digitalWrite(PIN_DATA_OUT[j], HIGH);
        else
          digitalWrite(PIN_DATA_OUT[j], LOW);
      }
  
      digitalWrite(PIN_CLOCKS[i], HIGH);
      digitalWrite(PIN_CLOCKS[i], LOW);
    }
    digitalWrite(PIN_TRANSISTORS[k], HIGH);
    delay(2);
    digitalWrite(PIN_TRANSISTORS[k], LOW);
  }
}

void flashLosingEdge(int player, int t)
{
  switch (player)
  {
    case 1:
      if (t%2000 < 1000)
        for (int i=0; i<8; i++)
          board[0][i][0] = board[0][i][0] || i%2;
      else
        for (int i=0; i<8; i++)
          board[0][i][0] = board[0][i][0] || 1-(i%2);
      break;
    case 2:
      if (t%2000 < 1000)
        for (int i=0; i<8; i++)
          board[8][i][0] = board[8][i][0] || i%2;
      else
        for (int i=0; i<8; i++)
          board[8][i][0] = board[8][i][0] || 1-(i%2);
      break;
    case 3:
      if (t%2000 < 1000)
        for (int i=0; i<8; i++)
          board[i][0][0] = board[i][0][0] || i%2;
      else
        for (int i=0; i<8; i++)
          board[i][0][0] = board[i][0][0] || 1-(i%2);
      break;
    case 4:
      if (t%2000 < 1000)
        for (int i=0; i<8; i++)
          board[i][8][0] = board[i][8][0] || i%2;
      else
        for (int i=0; i<8; i++)
          board[i][8][0] = board[i][8][0] || 1-(i%2);
      break;
    default:
    break;
  }
}



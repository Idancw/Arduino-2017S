#include "Arduino.h"
#include "Ball.h"
#include "Paddle.h"
#include "HardwareSerial.h"


// DEBUGGING
const int DEBUG = 1;    // 1 = flash every other light
const int USE_OLD_LIGHTING = 1;   // 1 = simple, zig-zagging method
const int LIGHT_ALL = 0;    // 1 = just light everything up all the time
const int AI_MODE = 1;

// == GAME CONSTANTS ==
double SPEEDUP = 1.005;
int players = 0;
int active_players = 0;

// == Settings ==
bool speedUpBall = true;
int ai_mode = AI_MODE;



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
Paddle p1 = Paddle(pL,pW,L,H);
Paddle p2 = Paddle(pL,pW,L,H);
Paddle p3 = Paddle(pL,pW,L,H);
Paddle p4 = Paddle(pL,pW,L,H);
Paddle* ps[4] = {&p1,&p2,&p3,&p4};

int board[8][8][8];
String msg[4] = {"", "", "", ""};
HardwareSerial* serials[4] = {&Serial, &Serial1, &Serial2, &Serial3};
//HardwareSerial* serials[4] = {&Serial, &Serial, &Serial, &Serial};

void getInput();


void setup()
{
  for (int i=0; i < 8; i++)
  {
    pinMode(PIN_DATA_OUT[i], OUTPUT);
    pinMode(PIN_CLOCKS[i], OUTPUT);
    pinMode(PIN_TRANSISTORS[i], OUTPUT);
  }

  for (int i=0; i<4; i++){
    serials[i]->begin(9600); //set baud rate
//    if (i == 2 || i == 3)
//      ps[i]->setActive(true);
  }
  gamestatus = -1 + AI_MODE;
//  gamestatus = 0;
  ball.reset();
}


void loop()
{
  getInput();
  
  if (gamestatus == 0)
  {
    drawBoard();
    showBoard();
    
    ball.go();

    if (ai_mode)
    {
      p1.go(ball.y-pL/2, ball.z-pW/2);    // Follow the ball
      p2.go(L-ball.y-pL/2, ball.z-pW/2);    // Follow the ball
      p3.go(W-ball.x-pL/2, ball.z-pW/2);    // Follow the ball
      p4.go(ball.x-pL/2, ball.z-pW/2);    // Follow the ball
    }
    
    gamestatus = ball.checkBounce(p1, p2, p3, p4);

    // Every second, increase speed by 1%
    if (t % 100 == 0)
    {
      if (speedUpBall)
        ball.speedUp(SPEEDUP);

//      if (shrinkPaddles)
//      {
//        for (int i=0; i<4; i++)
//          ps[i]->shrink(0.999);
//      }
    }
    if (gamestatus > 0)   // Game is over
    {
      serials[gamestatus-1]->print("x");
      Serial.println("Player " + String(gamestatus) + " has lost");
    }
  }
  else if (gamestatus > 0)
  {
    // Clear board
    for (int i=0; i<8; i++)
      for (int j=0; j<8; j++)
        for (int k=0; k<8; k++)
          board[i][j][k] = 0;
    flashLosingEdge(gamestatus);
//if (t%20 > 10)
    printPaddles();
if (t%20 < 10)
    printBall();
    showBoard();
//    delay(2);
  }
  else // Paused
  {
    drawBoard();
    showBoard();
  }
  t++;
}

void getInput()
{
  for (int i=0; i<4; i++)
  {
    if (serials[i]->available())
    {
      char ch = char(serials[i]->read());
      msg[i] += ch;
      if (ch == '\n')
      {
        switch (msg[i][0])
        {
          case 'c':   // Move paddle
            parseInput(i, msg[i].substring(1));
            break;
          case 's':   // Start game
            if (gamestatus == 0)
              break;
            ball.reset();
            gamestatus = 0;
            ai_mode = 0;
            break;
          case 'r':   // Reset game
            ball.reset();
            gamestatus = -1;
            ai_mode = 0;
            break;
          case 'a':   // AI Mode
            if (!ai_mode && active_players != 0)
              break;
            ball.reset();
            if (ai_mode == 0)
            {
              ai_mode = 1;
              gamestatus = 0;
            }
            else
            {
              ai_mode = 0;
              gamestatus = -1;
            }
            break;
          case 'j':   // Join game
            if (gamestatus != -1 && ai_mode == false)
              break;
            ai_mode == 0;
            active_players++;
            ps[i]->setActive(true);
            break;
          case 'q':   // Quit game
            if (ps[i]->activated != true)
              break;
            ps[i]->setActive(false);
            active_players--;
            break;
          default:
            parseInput(i, msg[i]);    // TODO: Remove when app is updated with msgs.
//            Serial.print("Bad input: " + msg[i]);
        }
        Serial.print(msg[i]);
        msg[i] = "";
      }
    }
  }
}

void parseInput(int p, String str)
{
  if (str[0] < '0' || str[0] > '0'+L-1 ||
      str[0] < '0' || str[0] > '0'+H-1 ||
      str[2] != '\n')
  {
    Serial.println("Bad Coords: " + str);
    return;
  }
  int x = str[0] - '0';
  int y = str[1] - '0';
  ps[p]->go(x,y);
}

void drawBoard()
{
  // Clear board
  for (int i=0; i<8; i++)
    for (int j=0; j<8; j++)
      for (int k=0; k<8; k++)
        board[i][j][k] = 0;//(i+j+k)%2;
  
//  board[0][0][0] = 1;   // For showing orientation

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
  if (p1.activated || ai_mode)
    for (int i=max(0,p1.x); i<=min(7,p1.x+p1.pLen); i++)
      for (int j=max(0,p1.y); j<=min(7,p1.y+p1.pWid); j++)
          board[0][i][j] = 1;
  
  if (p2.activated || ai_mode)
    for (int i=max(0,int(p2.x+0.5)); i<=min(7,int(p2.x+p2.pLen+0.5)); i++)
      for (int j=max(0,int(p2.y+0.5)); j<=min(7,int(p2.y+p2.pWid+0.5)); j++)
          board[7][7-i][j] = 1;
  
  if (p3.activated || ai_mode)
    for (int i=max(0,int(p3.x+0.5)); i<=min(7,int(p3.x+p3.pLen+0.5)); i++)
      for (int j=max(0,int(p3.y+0.5)); j<=min(7,int(p3.y+p3.pWid+0.5)); j++)
          board[7-i][0][j] = 1;
  
  if (p4.activated || ai_mode)
    for (int i=max(0,int(p4.x+0.5)); i<=min(7,int(p4.x+p4.pLen+0.5)); i++)
      for (int j=max(0,int(p4.y+0.5)); j<=min(7,int(p4.y+p4.pWid+0.5)); j++)
          board[i][7][j] = 1;
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
  for (int k=0; k<8; k++)
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
    delay(1);
    digitalWrite(PIN_TRANSISTORS[k], LOW);
  }
}

void flashLosingEdge(int player)
{
  switch (player)
  {
    case 1:
      if ((t%100) > 50)
        for (int i=0; i<8; i++)
          for (int j=0; j<8; j++)
          board[0][i][j] += 1;
      break;
    case 2:
      if ((t%100) > 50)
        for (int i=0; i<8; i++)
          for (int j=0; j<8; j++)
          board[7][i][j] += 1;
      break;
    case 3:
      if ((t%100) > 50)
        for (int i=0; i<8; i++)
          for (int j=0; j<8; j++)
          board[i][0][j] += 1;
      break;
    case 4:
      if ((t%100) > 50)
        for (int i=0; i<8; i++)
          for (int j=0; j<8; j++)
          board[i][7][j] = 1;
      break;
    default:
    break;
  }
}



#include "Ball.h"
#include "Paddle.h"

// == GAME CONSTANTS ==
int SPEEDUP = 1.05;


// pin connected to ST_CP of 74HC595
const byte PIN_CLOCK_7 = 9;
// pin connected to SH_CP of 74HC595
const byte PIN_CLOCK_6 = 8;
// pin connected to DS of 74HC595
byte PIN_DATA_OUT[8];

int t = 0;
int gamestatus = -1;  // 0 = playing
                      // [1,4] = that player has just lost.
                      // -1 = paused
int L=2,W=8,H=8;
Ball ball(0.50,L,W,H);
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
  for( int i=0 ; i < 8 ; ++i){
    PIN_DATA_OUT[i] = i;
    pinMode(PIN_DATA_OUT[i], OUTPUT);
  }
  pinMode(PIN_CLOCK_6, OUTPUT);
  pinMode(PIN_CLOCK_7, OUTPUT);
  
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
}


void drawBoard()
{
  // Clear board
  for (int i=0; i<8; i++)
    for (int j=0; j<8; j++)
      for (int k=0; k<8; k++)
        board[i][j][k] = 0; //(i+j+k)%2;

  float r = ball.getR();
  float x = ball.getX();
  float y = ball.getY();
  float z = ball.getZ();

  Serial.println("Ball's dim are:\t" + String(round(x-r)) + "-" + round(x+r) + "\t" +
                  round(y-r) + "-" + round(y+r) + "\t" +
                  round(z-r) + "-" + round(z+r));

  // Light pixels that are +-0.5 from any piece of the ball.
  for (int i=max(0,int(x-r+0.5)); i<=min(7,int(x+r+0.5)); i++)
    for (int j=max(0,int(y-r+0.5)); j<=min(7,int(y+r+0.5)); j++)
      for (int k=0; k<min(8,8); k++)
        board[i][j][k] = 1;
  

  // Actually show board
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

    if (i==0)
    {
      digitalWrite(PIN_CLOCK_6, HIGH);
      digitalWrite(PIN_CLOCK_6, LOW);
    }
    
    if (i==1)
    {
      digitalWrite(PIN_CLOCK_7, HIGH);
      digitalWrite(PIN_CLOCK_7, LOW);
    }
  
  }
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

// == CONSTANTS ==
// pin connected to ST_CP of 74HC595
const byte PIN_LATCH = 11;
// pin connected to SH_CP of 74HC595
const byte PIN_CLOCK = 12;
// pin connected to DS of 74HC595
const byte PIN_DATA0 = 5;
const byte PIN_DATA1 = 6;
const byte PIN_DATA2 = 7;
const byte PIN_DATA3 = 8;

const byte PIN_MUX0 = 0;
const byte PIN_MUX1 = 1;
const byte PIN_MUX2 = 2;


const byte DATAPINS[] = {PIN_DATA0, PIN_DATA1, PIN_DATA2, PIN_DATA3};

// -- DELAY RANGE --
const int MIN_DELAY = 50;
const int MAX_DELAY = 500;
 
// == GLOBAL DATA ==
byte val = B10;
byte dir_forward = 1;

int board[4][4]= {{1,1,1,1},
                  {1,1,1,1},
                  {1,1,1,1},
                  {1,1,1,1}};

void setup() 
{
  //set pins to output so you can control the shift register
  pinMode(PIN_LATCH, OUTPUT);
  pinMode(PIN_CLOCK, OUTPUT);
  pinMode(PIN_DATA0, OUTPUT);
  pinMode(PIN_DATA1, OUTPUT);
  pinMode(PIN_DATA2, OUTPUT);
  pinMode(PIN_DATA3, OUTPUT);

  
  pinMode(PIN_MUX0, OUTPUT);
  pinMode(PIN_MUX1, OUTPUT);
  pinMode(PIN_MUX2, OUTPUT);
 
  // the LEDs don't change while you're sending in bits
  digitalWrite(PIN_LATCH, LOW);
  // shift out the bits
  shiftOut(PIN_DATA0, PIN_CLOCK, MSBFIRST, B0000);
  shiftOut(PIN_DATA1, PIN_CLOCK, MSBFIRST, B0000);
  shiftOut(PIN_DATA2, PIN_CLOCK, MSBFIRST, B0000);
  shiftOut(PIN_DATA3, PIN_CLOCK, MSBFIRST, B0000);
  //take the latch pin high so the LEDs will light up
  digitalWrite(PIN_LATCH, HIGH);

  digitalWrite(PIN_MUX0, HIGH);
  digitalWrite(PIN_MUX1, LOW);
  digitalWrite(PIN_MUX2, LOW);
}

void loop()
{
  // -- read the value from the potentiometer and convert it to delay time -- 
  
  //int pot_val = analogRead(A5);
  //int delay_time = map(pot_val, 0, 1023, MIN_DELAY, MAX_DELAY);   
  int delay_time = 250;


//  printBoard();
  
  wander();


//  delay(delay_time);

  return;
  
  // take the PIN_LATCH low so LEDs don't change while you're sending in bits
  digitalWrite(PIN_LATCH, LOW);

  // send the bits
  shiftOut(PIN_DATA0, PIN_CLOCK, MSBFIRST, val); //val

  // -- set next LED on --
  if (dir_forward)
  {
    // val = val * 2 OR move 1 bit left
    val <<= 1;

    // 128 is 1000000, reached last LED -> revert the sequence
    // this jumps back and forth and always leaves the edge bits lit
    if(B10000 == val)
      dir_forward = 0;
  }
  else
  {
    // val = val / 2 OR move 1 bit right
    val >>= 1;

    // back to the first LED -> revert the sequence
    if(B00010 == val)
      dir_forward = 1;
  }

  // take the latch pin high so the LEDs will light up
  digitalWrite(PIN_LATCH, HIGH);

  // pause before next cycles
  delay(delay_time);
}

void printBoard()
{
  // take the PIN_LATCH low so LEDs don't change while you're sending in bits
  
  for (int i = 0; i < 4; i++)
  {
    digitalWrite(PIN_LATCH, LOW);

    // send the bits
    shiftOut(DATAPINS[i], PIN_CLOCK, MSBFIRST, rowToInt(i)*2); //val
    // take the latch pin high so the LEDs will light up
    digitalWrite(PIN_LATCH, HIGH);
    delay(3);
  }
}

int rowToInt(int row)
{
  int val = 0;
  for (int i = 0; i < 4; i++)
  {
    val *= 2;
    val += board[row][i];
  }
  return val;
}

void printBall(int i, int j, int k)
{
  digitalWrite(PIN_LATCH, LOW);
  if (k == 0)
    shiftOut(DATAPINS[i], PIN_CLOCK, MSBFIRST, jToVal(j)); //2^(j+1)
  else
    shiftOut(DATAPINS[i+2], PIN_CLOCK, MSBFIRST, jToVal(j)); //2^(j+1)
  
  digitalWrite(PIN_LATCH, HIGH);  
}

int jToVal(int j)
{
  int r = 0.5 + pow(2,j);
  r <<=1;
  return r;
  // TODO: Use pow() - return 2**(j+1);
  if (j == 0) return 2;
  if (j == 1) return 4;
  if (j == 2) return 8;
  if (j == 3) return 16;
  else return B0110;
}

void wander()
{
  for (int a = 0; a<2; a++)
    for (int b = 0; b<4; b++)
      for (int c = 0; c<2; c++)
      {
        printBall(a,b,c);
        delay(500);
      }
  return;

  int i=1, j=1, k=1;
  while (true)
  {

    printBall(i,j,k);
    delay(300);

    int ni=i, nj=j, nk=k;
    while ((ni==i && nj==j && nk==k))
    {
      ni += random(-1,2);
      if (ni<0) ni=0;
      if (ni>1) ni=1;
  
      nj += random(-1,2);
      if (nj<0) nj=0;
      if (nj>3) nj=3;
      
      nk += random(-1,2);
      if (nk<0) nk=0;
      if (nk>1) nk=1;
    }
    i=ni;
    j=nj;
    k=nk;
  }
}






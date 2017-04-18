// == CONSTANTS ==
// pin connected to ST_CP of 74HC595
const byte PIN_LATCH = 11;
// pin connected to SH_CP of 74HC595
const byte PIN_CLOCK = 12;
// pin connected to DS of 74HC595
const byte PIN_DATA = 8;
 
// -- DELAY RANGE --
const int MIN_DELAY = 50;
const int MAX_DELAY = 500;
 
// == GLOBAL DATA ==
byte val = B10;
byte dir_forward = 1;

void setup() 
{
  //set pins to output so you can control the shift register
  pinMode(PIN_LATCH, OUTPUT);
  pinMode(PIN_CLOCK, OUTPUT);
  pinMode(PIN_DATA, OUTPUT);
 
  // the LEDs don't change while you're sending in bits
  digitalWrite(PIN_LATCH, LOW);
  // shift out the bits
  shiftOut(PIN_DATA, PIN_CLOCK, MSBFIRST, 0);
  //take the latch pin high so the LEDs will light up
  digitalWrite(PIN_LATCH, HIGH);
}

void loop() 
{
    // -- read the value from the potentiometer and convert it to delay time -- 
    
    //int pot_val = analogRead(A5);
    //int delay_time = map(pot_val, 0, 1023, MIN_DELAY, MAX_DELAY);   
    int delay_time = 50;
 
    // take the PIN_LATCH low so LEDs don't change while you're sending in bits
    digitalWrite(PIN_LATCH, LOW);
    digitalWrite(PIN_CLOCK, LOW);

    // send the bits
    shiftOut(PIN_DATA, PIN_CLOCK, MSBFIRST, B10000001 + val); //val

    // -- set next LED on --
    if (dir_forward)
    {
      // val = val * 2 OR move 1 bit left
      val <<= 1;
 
      // 128 is 1000000, reached last LED -> revert the sequence
      // this jumps back and forth and always leaves the edge bits lit
      if(B01000000 == val)
        dir_forward = 0;
    }
    else
    {
      // val = val / 2 OR move 1 bit right
      val >>= 1;
 
      // back to the first LED -> revert the sequence
      if(B00000010 == val)
        dir_forward = 1;
    }
 
    // take the latch pin high so the LEDs will light up
    digitalWrite(PIN_LATCH, HIGH);
 
    // pause before next cycles
    delay(delay_time);
}

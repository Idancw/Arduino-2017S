// anodes
int row[] = {5,6,7};
// cathodes
int col[] = {8,9,10};

// bit patterns for each row
byte pattern1_data[] = {
  0,0,0};

byte pattern2_data[] = {
  0,0,0};

byte pattern3_data[] = {
  0,0,0};

byte pattern4_data[] = {
  0,0,0};
  
byte pattern5_data[] = {
  0,0,0};

// defines the size of the matrix 
int columns = 3;
int rows = 3;

//millisecond delay between displaying each row
int pause = 1;



void setup()
{
  for (int i=0;i<3;i++)
  {
    pinMode(row[i], OUTPUT);
    pinMode(col[i], OUTPUT);
  } 
  allOff();
  randomSeed(analogRead(A0)); // uses seed from reading Voltage from unused pin.
}


void loop()
{
  wander();
  
//  // define pattern
//  pattern1_data[0] = B00000101;
//  pattern1_data[1] = B00000010;
//  pattern1_data[2] = B00000101;
//
//  pattern2_data[0] = B00000000;
//  pattern2_data[1] = B00000111;
//  pattern2_data[2] = B00000000;
//
//  pattern3_data[0] = B00000101;
//  pattern3_data[1] = B00000101;
//  pattern3_data[2] = B00000101;
//
//  pattern4_data[0] = B00000010;
//  pattern4_data[1] = B00000010;
//  pattern4_data[2] = B00000010;
//
//  pattern5_data[0] = B00000010;
//  pattern5_data[1] = B00000011;
//  pattern5_data[2] = B00000100;
//  
//  
//  int msCounter = 0;
//
//  while (msCounter < 100 ) {
//    showPattern(pattern1_data);
//    delay(10);
//    msCounter++;
//  }
//
//  msCounter = 0;
//  allOff();
//
//  while (msCounter < 100 ) {
//    
//    showPattern(pattern2_data);
//     delay(10);
//    msCounter++;
//  }
//
//  msCounter = 0;
//  allOff();
//
//  while (msCounter < 100 ) {
//    
//    showPattern(pattern3_data);
//     delay(10);
//    msCounter++;
//  }
//
//  msCounter = 0;
//  allOff();
//
//  while (msCounter < 100 ) {
//    
//    showPattern(pattern4_data);
//     delay(10);
//    msCounter++;
//  }
//
//  
//  msCounter = 0;
//  allOff();
//
//  while (msCounter < 100 ) {
//    
//    showPattern(pattern5_data);
//     delay(10);
//    msCounter++;
//  }
  
  
}

void allOff()
{
  for (int i=0;i<3;i++)
  {
    digitalWrite(row[i], LOW);
    digitalWrite(col[i], HIGH);
  }
}


void showPattern(byte data[])
{
  for (int thisrow=0;thisrow<rows;thisrow++)
  {
    //turn everything off
    allOff();
    //turn on current row
    digitalWrite(row[thisrow], HIGH);
    // loop through columns of this row and light
    for (int thiscol=0;thiscol<columns;thiscol++)
    {
      if (bitRead(data[thisrow],columns-thiscol-1)==1)
      {
        digitalWrite(col[thiscol], LOW);
      }
      else
      {
        digitalWrite(col[thiscol], HIGH);
      }
    }
    delay(pause);
  }
}


void showPattern2()
{
  digitalWrite(row[1], HIGH);
  digitalWrite(col[1], LOW);
}


void showPattern3()
{
  for (int thisrow=0;thisrow<rows;thisrow++)
  {
    //turn everything off
    allOff();
    //turn on current row
    digitalWrite(row[thisrow], HIGH);
    // loop through columns of this row and light
    for (int thiscol=0;thiscol<columns;thiscol++)
    {
      if (bitRead(pattern3_data[thisrow],columns-thiscol-1)==1)
      {
        digitalWrite(col[thiscol], LOW);
      }
      else
      {
        digitalWrite(col[thiscol], HIGH);
      }
    }
    delay(pause);
  }
}

void wander()
{
  int i=1, j=1;
  while (true)
  {
    //turn everything off
    allOff();
    
    digitalWrite(row[i], HIGH);
    digitalWrite(col[j], LOW);
    delay(500);

    int ni=i, nj=j;
    while ((ni==i && nj==j))
    {
      ni += random(0,2)*2 -1;
      if (ni<0) ni=0;
      if (ni>2) ni=2;
  
      nj += random(0,2)*2 - 1;
      if (nj<0) nj=0;
      if (nj>2) nj=2;
    }
    i=ni;
    j=nj;
  }
}





// == CONSTANTS ==
// pin connected to ST_CP of 74HC595
const byte PIN_CLOCK_7 = 9;
// pin connected to SH_CP of 74HC595
const byte PIN_CLOCK_6 = 8;
// pin connected to DS of 74HC595
byte PIN_DATA_OUT[8];
byte data_reg_1[8] = {0,0,0,0,0,0,1,1};
int cnt = 0;

// pin connected to DMUX DATA
const byte PIN_REG_DMUX0 = 10;
// pin connected to DMUX DATA
const byte PIN_REG_DMUX1 = 11;
// pin connected to DMUX DATA
const byte PIN_REG_DMUX2 = 12;
// pin connected to DMUX COM IN/OUT
const byte PIN_REG_DMUX_COM = 13;


void setup() {
  // put your setup code here, to run once:
  for( int i=0 ; i < 8 ; ++i){
    PIN_DATA_OUT[i] = i;
    pinMode(PIN_DATA_OUT[i], OUTPUT);
  }
  pinMode(PIN_CLOCK_6, OUTPUT);
  pinMode(PIN_CLOCK_7, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  
//  digitalWrite(PIN_CLOCK, HIGH);
  
  for( int i=0 ; i < 8 ; ++i) {
    if (i==cnt%8)
      digitalWrite(PIN_DATA_OUT[i], HIGH);    
    else
      digitalWrite(PIN_DATA_OUT[i], LOW);
  }

  digitalWrite(PIN_CLOCK_6, HIGH);
  digitalWrite(PIN_CLOCK_6, LOW);
  
  for( int i=0 ; i < 8 ; ++i) {
    if (7-i==cnt%8)
      digitalWrite(PIN_DATA_OUT[i], HIGH);    
    else
      digitalWrite(PIN_DATA_OUT[i], LOW);
  }
  digitalWrite(PIN_CLOCK_7, HIGH);
  digitalWrite(PIN_CLOCK_7, LOW);

  cnt++;
  delay(500);


}

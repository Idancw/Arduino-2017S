// == CONSTANTS ==
// pin connected to ST_CP of 74HC595
const byte PIN_ENABLE = 9;
// pin connected to SH_CP of 74HC595
const byte PIN_CLOCK = 8;
// pin connected to DS of 74HC595
byte PIN_DATA_OUT[8];
byte data_reg_1[8] = {0,1,0,1,0,1,0,1};
int cnt = 0;

void setup() {
  // put your setup code here, to run once:
  for( int i=0 ; i < 8 ; ++i){
    PIN_DATA_OUT[i] = i;
  }
  pinMode(PIN_DATA_OUT[0], OUTPUT);
  pinMode(PIN_DATA_OUT[1], OUTPUT);
  pinMode(PIN_DATA_OUT[2], OUTPUT);
  pinMode(PIN_DATA_OUT[3], OUTPUT);
  pinMode(PIN_DATA_OUT[4], OUTPUT);
  pinMode(PIN_DATA_OUT[5], OUTPUT);
  pinMode(PIN_DATA_OUT[6], OUTPUT);
  pinMode(PIN_DATA_OUT[7], OUTPUT);
  pinMode(PIN_CLOCK, OUTPUT);
  pinMode(PIN_ENABLE, OUTPUT);  
  digitalWrite(PIN_ENABLE, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  
//  digitalWrite(PIN_CLOCK, HIGH);

  digitalWrite(PIN_CLOCK, LOW);
  digitalWrite(PIN_CLOCK, HIGH);  
  for( int i=0 ; i < 8 ; ++i){
     if ( cnt%2 == 0) {
      if (data_reg_1[i]==1 ){
        digitalWrite(PIN_DATA_OUT[i], HIGH);    
      } else{
        digitalWrite(PIN_DATA_OUT[i], LOW);
      }
     } else {
        if (data_reg_1[i]==0 ){
        digitalWrite(PIN_DATA_OUT[i], HIGH);    
      } else{
        digitalWrite(PIN_DATA_OUT[i], LOW);
      }
     }
  }
  cnt++;
  delay(150);
}

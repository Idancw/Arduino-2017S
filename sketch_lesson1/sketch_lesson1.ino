#include <Servo.h>

int led = 13;
int motor = 9;
int button = 7;
int servo = 10;
int buttonState;
int oldState = 0;
int s = 0;
int dial = A0;
int mspeed;
const int MAX_SPEED = 1023;
int pos = 0;
int goingUp = 1;

Servo myservo;


void setup() {
  // put your setup code here, to run once:
  pinMode(led,OUTPUT);
  pinMode(motor,OUTPUT);
  pinMode(button,INPUT);
  pinMode(dial,INPUT);

  //int srialVal=100;
  Serial.begin(1200);
  
  myservo.attach(servo); 

}

void loop() {
  // put your main code here, to run repeatedly:

  Serial.println(float(mspeed)/MAX_SPEED);
  
  mspeed = analogRead(dial);
  buttonState = digitalRead(button);
  if ((buttonState != oldState) && (buttonState == 0)){
    s = 1 - s;
  }
  analogWrite(motor, 1000);
  
  if (s) {
    analogWrite(motor, mspeed);
    digitalWrite(led, HIGH);
    
    if (goingUp && pos < 180) {
      myservo.write(pos);
      pos += 5;
    }
    else {
      myservo.write(pos);
      pos -= 5;
      goingUp = 0;
    }
    if (pos == 0)
      goingUp = 1;
    
//    delay(5);
  }
  else{
    digitalWrite(motor, LOW);
    digitalWrite(led, LOW);
    myservo.write(0);
   // delay(10);  
  }
  oldState = buttonState;
//  delay(10);
}

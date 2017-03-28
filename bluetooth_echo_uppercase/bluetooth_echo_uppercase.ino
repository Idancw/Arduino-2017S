String message; //string that stores the incoming message
char ch;

void setup()
{
  Serial.begin(9600); //set baud rate
}

void loop()
{
  while(Serial.available())
  {//while there is data available on the serial monitor
    ch = char(Serial.read());//store string from serial command
    if (ch>=97)
      ch -= ('a'-'A');
    message+=ch;
  }
  if(!Serial.available())
  {
    if(message!="")
    {//if data is available
      Serial.println(message); //show the data
      message=""; //clear the data
    }
  }
  delay(200); //delay
}
    

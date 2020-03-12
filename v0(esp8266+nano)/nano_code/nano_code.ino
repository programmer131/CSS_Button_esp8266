#include <ArduinoJson.h>

// constants won't change. Used here to set a pin number :
const int ledPin =  LED_BUILTIN;// the number of the LED pin

char serialRxBuf[20];

void readIOSatus(){  
  Serial.print(digitalRead(ledPin)+0);
  Serial.print('\n');
}

void runPeriodicFunc()
{
  static const unsigned long REFRESH_INTERVAL1 = 200,REFRESH_INTERVAL2 = 600; // ms
  static unsigned long lastRefreshTime1 = 0,lastRefreshTime2 = 0;
  
  if(millis() - lastRefreshTime1 >= REFRESH_INTERVAL1)
  {     
    readIOSatus();     
   // sendNanoStatus();        
    lastRefreshTime1 = millis() ;
  }
  if(millis() - lastRefreshTime2 >= REFRESH_INTERVAL2)
  {
  //  readAnalogStatus();    
    lastRefreshTime1 = millis() ;
    lastRefreshTime2 = millis() ;                
  }
}

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(57600);
  Serial.setTimeout(1);
  readIOSatus();
  //readAnalogStatus();  
}


void serialEvent(){
  static unsigned char index=0;
  while(Serial.available()){    
    char inChar = (char)Serial.read();        
    if (inChar == '\n') {   
      serialRxBuf[index]=0x00;      
      index=0;
      String espData(serialRxBuf);      
      if(espData.equals("toggle"))
      {
        digitalWrite(ledPin,!digitalRead(ledPin));      
      }
      readIOSatus();            
    }
    else
    {
      serialRxBuf[index++]= inChar;
    }
  }
}


void loop() {
  runPeriodicFunc();
  serialEvent();     
}


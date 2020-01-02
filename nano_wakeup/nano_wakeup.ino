/*
  Blink

  Turns an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino
  model, check the Technical Specs of your board at:
  https://www.arduino.cc/en/Main/Products

  modified 8 May 2014
  by Scott Fitzgerald
  modified 2 Sep 2016
  by Arturo Guadalupi
  modified 8 Sep 2016
  by Colby Newman

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/Blink
*/

#include <avr/power.h> // don't forget this!

//#ifdef F_CPU
//#undef F_CPU
//#define F_CPU (16000000UL / 32)
//#endif

// the setup function runs once when you press reset or power the board
void setup() {
  //clock_prescale_set (clock_div_32); 
  pinMode(LED_BUILTIN, OUTPUT);
  
}

double mapf(double val, double in_min, double in_max, double out_min, double out_max) {
    return (val - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float getVoltage()
{
  int i = analogRead(A6);
  return mapf(i, 111, 220, 6.0f, 12.0f);
}

void ledBlink(unsigned long period, int count)
{
  for (int i = 0; i < count; i++)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(period/2);
    digitalWrite(LED_BUILTIN, LOW);
    delay(period/2);
  }
}

void delayWithData(unsigned long ms)
{
  unsigned long timestamp = millis() + ms;
  Serial.begin(115200);
  while(millis() <= timestamp)
  {
    delay(500);
    Serial.println(getVoltage());
  }
  Serial.end();
}

void rpiPower(bool enable)
{
  if(enable)
  {
    digitalWrite(2, HIGH);
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    digitalWrite(2, LOW);
    digitalWrite(LED_BUILTIN, LOW);
  }
}

void loop() {

  if(getVoltage() > 10.8f)
  {
    rpiPower(true); 
    delayWithData(60000);
  }
  else
  {
    ledBlink(200, 6);
  }
  
  rpiPower(false);
  delay(4000);    
}

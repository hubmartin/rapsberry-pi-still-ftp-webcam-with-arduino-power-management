/*

  Solar-powered Raspberry Pi picture camera with Arduino for power-on/off

  martinhubacek.cz
  https://github.com/hubmartin/rapsberry-pi-still-ftp-webcam-with-arduino-power-management
  
*/

#include <avr/power.h>
//#ifdef F_CPU
//#undef F_CPU
//#define F_CPU (16000000UL / 32)
//#endif

#define RPI_ON_PERIOD_MS (60 * 1000ul)
#define RPI_OFF_PERIOD_MS (15 * 60ul * 1000ul)
#define SAFETY_CUTOFF_VOLTAGE 10.8f

float voltage_unloaded = 0.0f;

// the setup function runs once when you press reset or power the board
void setup() {
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
    Serial.print(getVoltage());
    Serial.print(",");
    Serial.println(voltage_unloaded);
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

  voltage_unloaded = getVoltage();
  if(voltage_unloaded > SAFETY_CUTOFF_VOLTAGE)
  {
    rpiPower(true); 
    delayWithData(RPI_ON_PERIOD_MS);
  }
  else
  {
    ledBlink(200, 6);
  }
  
  rpiPower(false);

  // slow MCU down 128x times to save some power
  // Improvements with sleep and deepsleep&edg are welcome
  clock_prescale_set (clock_div_128); 
  delay(RPI_OFF_PERIOD_MS / 128);    
  clock_prescale_set (clock_div_1); 
}

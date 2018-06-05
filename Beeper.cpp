#include "Beeper.h"
#include <Arduino.h>

void Beeper::begin(int speaker_pin)
{
  pin = speaker_pin;
  pinMode(pin, OUTPUT);
  
}

void Beeper::beep(int ms)
{
  digitalWrite(pin, HIGH);
  delay(ms);
  digitalWrite(pin, LOW);
}

void Beeper::beep3(void)
{
  digitalWrite(pin, HIGH);
  delay(100);
  digitalWrite(pin, LOW);
  delay(100);
  digitalWrite(pin, HIGH);
  delay(100);
  digitalWrite(pin, LOW);
  delay(100);
  digitalWrite(pin, HIGH);
  delay(100);
  digitalWrite(pin, LOW);
  delay(100);
}

void Beeper::countdown(int mins)
{
  for(int i = 0; i < mins; i++)
  {
    beep(1000);
    delay(60000);
  }
  beep3();
}

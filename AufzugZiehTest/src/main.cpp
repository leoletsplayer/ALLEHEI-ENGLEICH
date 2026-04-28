#include <Arduino.h> 
#define SCHRITTMOTOR GPIOC->ODR

unsigned int motorCW[] = {0x300, 0x600, 0xc00, 0x900};
unsigned int anzahlSchritte = 0;

void setup()
{
  pinMode(PC8, OUTPUT);
  GPIOC->MODER = 0x550000;
  SCHRITTMOTOR = 0;
}

void loop()
{
  if (anzahlSchritte < 4000)
  {
    //Vorwaertslauf
    SCHRITTMOTOR = motorCW[anzahlSchritte%4]; // %4 wegen 4 Stellen des Arrays
    anzahlSchritte++;
    delay(2);
  }
  else 
  {
    //Stopp
    SCHRITTMOTOR = 0b0000<<8;
  }
}
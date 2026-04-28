#include <Arduino.h>

#define LEDS GPIOC->ODR
unsigned char schalter = PB0;
unsigned char licht[8] = {0x01, 0x04, 0x10, 0x40, 0x80, 0x20, 0x08, 0x02};

void setup() {
  pinMode(schalter, INPUT_PULLDOWN);
  RCC->AHBENR |= 0x04;
  GPIOC->MODER = 0x5555;
}

void loop() {
for (unsigned char i = 0; i < 8; i++)
{
  LEDS = licht[i];
  if (digitalRead(schalter) == HIGH) delay(100);
  else delay(500);
}

}


#include <Arduino.h>

#define LEDS GPIOC->ODR

void setup()
{
  RCC->AHBENR |= 0x04; /*alternativ: RCC->AHBENR |= 1<<2;*/
  GPIOC->MODER = 0x5555;
  GPIOC->ODR = 0x55;
}

void loop()
{
  LEDS = ~LEDS;
  delay(300);
}
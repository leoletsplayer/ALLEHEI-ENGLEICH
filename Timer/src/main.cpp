#include <Arduino.h>
#include <Wire.h>
#include "LiquidCrystal_PCF8574.h"
LiquidCrystal_PCF8574 lcd(0x27);

#define LED PC0
static HardwareTimer flip = HardwareTimer(TIM6);

void isr_blinken(){
  digitalWrite(LED,!digitalRead(LED));
}

void setup(){
  pinMode(LED,OUTPUT);
  flip.setPrescaleFactor(32000); 
  flip.setOverflow(300);
  flip.attachInterrupt(isr_blinken);
  flip.resume();4
  
}

void loop(){

}
#include <Arduino.h>
#include <Wire.h>
#include "LiquidCrystal_PCF8574.h"
LiquidCrystal_PCF8574 lcd(0x27);

#define ZAHL (GPIOB->IDR & 0xff)

void setup(){
    lcd.begin(16,2);
    lcd.setBacklight(255);
    lcd.clear();

    RCC->AHBENR |= 0x02;
    GPIOB->PUPDR = 0xaaaa;
}
 
void loop(){    
    unsigned char anzahl = 0;
    unsigned char maske = 1;
    for (unsigned char i = 0; i<8; i++){
            if ((ZAHL & maske) == maske) anzahl++;
            maske = maske<<1;
    }
    lcd.setCursor(0,0);
    lcd.printf("Anzahl 1er: %d", anzahl);
}


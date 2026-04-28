#include <Arduino.h>
#include <Wire.h>

#include "LiquidCrystal_PCF8574.h"
LiquidCrystal_PCF8574 display(0x27);

#define Adresse 0x4F
unsigned char temp_register = 0;
unsigned char temp_lsb;
signed char temp_msb;

void setup()
{
  display.begin(16,2);
  display.setBacklight(255);
  display.clear();
  display.setCursor(0,0);
  display.printf("it.schule");

  Wire.begin();
}

void loop()
{
  Wire.beginTransmission(Adresse);
  Wire.write(temp_register);
  Wire.endTransmission(false);

  Wire.requestFrom(Adresse, 2);
  temp_msb = Wire.read();
  temp_lsb = Wire.read();
  Wire.endTransmission();

  display.setCursor(0,1);
  display.printf("Temp: %d C*",temp_msb);
}
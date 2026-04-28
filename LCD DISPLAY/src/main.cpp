#include <Arduino.h>
#include <Wire.h>
#include "LiquidCrystal_PCF8574.h"
LiquidCrystal_PCF8574 lcd(0x27);

#define SCHALTER PB0
unsigned char wuerfel = 1;

byte ue[8] = {0b01010,
              0b00000,
              0b10001,
              0b10001,
              0b10001,
              0b10011,
              0b01101,
              0b00000}; 

void setup(){
  pinMode(SCHALTER, INPUT_PULLDOWN);
  lcd.begin(16, 2);
  lcd.setBacklight(255);
  lcd.setCursor(0,0);
  lcd.clear();
  lcd.createChar(0, ue); // benutzerdefiniertes Zeichen erstellen
}

void loop() {
 
  if (digitalRead(SCHALTER) == 1) {
   
    while (digitalRead(SCHALTER) == 1) {
      lcd.clear();
      lcd.printf("W");
      lcd.write(byte(0));
      lcd.printf("rfeln .");
      delay(400);
      lcd.clear();
      lcd.printf("W");
      lcd.write(byte(0));
      lcd.printf("rfeln ..");
      delay(400);
      lcd.clear();
      lcd.printf("W");
      lcd.write(byte(0));
      lcd.printf("rfeln ...");
      delay(400);
      lcd.clear();

    }
    wuerfel = random(1, 7);

    // Wenn losgelassen: Endergebnis anzeigen
    lcd.clear();
    lcd.printf("W");
    lcd.write(byte(0)); // kurz stehen lassen
    lcd.printf("rfeln: %d", wuerfel);
    delay(1000);
    double x = random(0, 1000) / 1000.0;//wenn ich jetzt hier was reinschreibe soll endlich es auch direkt gepusht werden
  }
}
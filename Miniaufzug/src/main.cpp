#include <Arduino.h>
#include <Wire.h>
#include "LiquidCrystal_PCF8574.h"
LiquidCrystal_PCF8574 lcd(0x27);

#define BUZZER PB5
#define c1 262
#define d1 294
#define e1 330
#define f1 349
#define g1 392
#define a1 440
#define h1 494
#define c2 523
#define PWM_RES 8
#define PWM_MAX 255

const int melodie[] = {c1, e1, g1,  e1, d1, c1, d1, f1, a1,  g1, e1, d1, c1, 0};
const int melodieDauer[] = {250, 250, 350, 250, 250, 350, 250, 250, 350, 450, 250, 250, 450, 250};
const int melodieLaenge = sizeof(melodie) / sizeof(melodie[0]);
int musik = 0;
const unsigned int SchritteProNote = 150;

void musicStep() {
  int f = melodie[musik];
  int d = melodieDauer[musik];

  if (f == 0) {
    noTone(BUZZER);
  } else {
    tone(BUZZER, f, d);
  }

  musik = (musik + 1) % melodieLaenge;
}
void musikStart() {
  musik = 0;
  noTone(BUZZER);
}
void musikStop() {
  noTone(BUZZER);
}


#define TasterEG  PB0
#define TasterOG1 PB3
#define TasterOG2 PB4

#define SCHRITTMOTOR GPIOC->ODR
volatile int ZielStockwerk = -1;
volatile bool faehrt = false;
unsigned int Stockwerk = 0;
unsigned int SchritteProStockwerk = 3100;

unsigned int motorCW[] = {0x300, 0x600, 0xc00, 0x900};
unsigned int motorCCW[] = {0x900, 0xc00, 0x600, 0x300};

void Motor_Hoch(unsigned int schritte){
  int punkt = 0;
  musikStart();

  for (unsigned int i = 0; i < schritte; i++)
  {
    SCHRITTMOTOR = motorCCW[i % 4];
    
    if (i % SchritteProNote == 0) {
      musicStep();
    }

    if (i % 400 == 0)
    {
      lcd.setCursor(0,0);
      if (punkt == 0){ 
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("F\xe1hrt hoch."); }
      if (punkt == 1){ 
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("F\xe1hrt hoch.."); }
      if (punkt == 2){ 
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("F\xe1hrt hoch..."); }

      punkt = (punkt + 1) % 3;
    }
    delay(2);
    
  }
  musikStop();
}
void Motor_Runter(unsigned int schritte){
  int punkt = 0;
  musikStart();

  for (unsigned int i = 0; i < schritte; i++)
  {
    SCHRITTMOTOR = motorCW[i % 4];
    
    if (i % SchritteProNote == 0) {
      musicStep();
    }

    if (i % 400 == 0)
    {
      lcd.setCursor(0,0);
      if (punkt == 0){ 
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("F\xe1hrt runter."); }
      if (punkt == 1){ 
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("F\xe1hrt runter.."); }
      if (punkt == 2){ 
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("F\xe1hrt runter..."); }
      punkt = (punkt + 1) % 3;
    }
    delay(2);  
  }
  musikStop();
}

void Anzeige_StockwerkWaehlen(){
  lcd.clear();
  lcd.setCursor(0,0);
  
  lcd.print("Bitte Stockwerk");
  lcd.setCursor(0,1);
  lcd.print("W\xe1hlen");
}

void isr_Erdgeschoss(){   if(!faehrt) {ZielStockwerk = 0; }}
void isr_Obergeschoss1(){ if(!faehrt) {ZielStockwerk = 1; }}
void isr_Obergeschoss2(){ if(!faehrt) {ZielStockwerk = 2; }}

void setup() {
  analogWriteResolution(PWM_RES);

  pinMode(TasterEG, INPUT_PULLDOWN);
  pinMode(TasterOG1, INPUT_PULLDOWN);
  pinMode(TasterOG2, INPUT_PULLDOWN);

  pinMode(PC8, OUTPUT);
  GPIOC->MODER = 0x550000;
  SCHRITTMOTOR = 0;

  lcd.begin(16,2);
  lcd.clear();
  lcd.setBacklight(255);
  lcd.setCursor(0,0);
  lcd.print("Bitte Stockwerk");
  lcd.setCursor(0,1);
  lcd.print("W\xe1hlen");
  
  attachInterrupt(digitalPinToInterrupt(TasterEG), isr_Erdgeschoss, RISING);
  attachInterrupt(digitalPinToInterrupt(TasterOG1), isr_Obergeschoss1, RISING);
  attachInterrupt(digitalPinToInterrupt(TasterOG2), isr_Obergeschoss2, RISING);

  ZielStockwerk = -1;
  delay(300);
}

void loop() {
  if (ZielStockwerk != -1)
  {
    faehrt = true;
    int ziel = ZielStockwerk;
    
    if (ziel == Stockwerk)
    {
      lcd.clear();
      lcd.print("Bereits in");
      lcd.setCursor(0,1);
      lcd.print("dem Stockwerk");
      delay(2000);
      Anzeige_StockwerkWaehlen();
      ZielStockwerk = -1;
      faehrt = false;
      return;
    }
    
    if (ziel > Stockwerk)
    {
      Motor_Hoch(SchritteProStockwerk * (ziel - Stockwerk));
      SCHRITTMOTOR = 0;
    } 
    else if (ziel < Stockwerk)
    {
      Motor_Runter(SchritteProStockwerk * (Stockwerk - ziel));
      SCHRITTMOTOR = 0;
    }


    Stockwerk = ziel;

    lcd.clear();
    if (Stockwerk == 0)
    {
      lcd.print("Angekommen im");
      lcd.setCursor(0,1);
      lcd.print("Erdgeschoss");
    }
    else
    {
      lcd.print("Angekommen im");
      lcd.setCursor(0,1);
      lcd.print("Obergeschoss ");
      lcd.print(Stockwerk);
    }
    
    delay(5000);
    faehrt = false;  
    Anzeige_StockwerkWaehlen();
    ZielStockwerk = -1;

  }
}
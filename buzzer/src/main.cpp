#include <Arduino.h>

/* =======================
   Frequenzen (gegeben)
   ======================= */
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

/* =======================
   Hardware
   ======================= */
#define BUZZER_PIN PB3   // TIM2_CH2

HardwareTimer *MyTim;

/* =======================
   Funktion: Ton spielen
   ======================= */
void playTone(int freq, int durationMs) {
  MyTim->setOverflow(freq, HERTZ_FORMAT);                 // Frequenz setzen
  MyTim->setCaptureCompare(2, 30, PERCENT_COMPARE_FORMAT); // 50% Duty
  MyTim->resume();                                        // PWM an

  delay(durationMs);

  MyTim->pause();   // PWM aus
  delay(60);        // kleine Pause (wichtig für sauberen Klang)
}

/* =======================
   Setup
   ======================= */
void setup() {
  pinMode(BUZZER_PIN, OUTPUT);

  MyTim = new HardwareTimer(TIM2);               // TIM2 existiert sicher
  MyTim->setMode(2, TIMER_OUTPUT_COMPARE_PWM1, BUZZER_PIN);
}

/* =======================
   Loop: Fahrstuhl-Musik
   ======================= */
void loop() {

  // Klassische, ruhige Fahrstuhl-Melodie
  playTone(c1, 400);
  playTone(e1, 400);
  playTone(g1, 600);

  playTone(e1, 400);
  playTone(d1, 400);
  playTone(c1, 600);

  playTone(d1, 400);
  playTone(f1, 400);
  playTone(a1, 600);

  playTone(g1, 800);

  delay(1500); // Pause bis zur Wiederholung
}
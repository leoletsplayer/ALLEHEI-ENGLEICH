#include <Arduino.h>

#define BTRX PB11 // HM-10 RxD (Nucleo sendet) -> UART RX-Pin fürs Objekt
#define BTTX PB10 // HM-10 TxD (Nucleo empfängt) -> UART TX-Pin fürs Objekt
#define LED  PA5  // Nucleo-Board-LED

#define PCRX PA3
#define PCTX PA2
#define BTSTATE PA1

#define SWITCH   PB0    // Schiebeschalter
#define BTENABLE PB12   // HM-10 Enable/BRK


HardwareSerial SerialBT(BTRX, BTTX);
HardwareSerial SerialPC(PCRX, PCTX);

int leds[] = {PC0, PC1, PC2, PC3};
bool lastBTState = false;

void setup() {
  pinMode(SWITCH, INPUT_PULLDOWN);  // falls dein Schalter gegen 3.3V schaltet
  pinMode(BTENABLE, OUTPUT);
  digitalWrite(BTENABLE, HIGH);     // wichtig: Standard = HIGH (nicht trennen)

  SerialPC.begin(9600);
  SerialBT.begin(9600);
  delay(100);
  pinMode(BTSTATE, INPUT);
  pinMode(LED, OUTPUT);
  pinMode(PC0, OUTPUT);
  pinMode(PC1, OUTPUT);
  pinMode(PC2, OUTPUT);
  pinMode(PC3, OUTPUT);

  digitalWrite(LED, 1);
}

void loop() {
  if (digitalRead(SWITCH) == HIGH) {
    digitalWrite(BTENABLE, LOW);    // BRK=0 -> Verbindung trennen / Konfigmodus
  } else {
    digitalWrite(BTENABLE, HIGH);   // normaler Betrieb
  }

  bool currentBTState = digitalRead(BTSTATE);
  if (currentBTState && !lastBTState) {
    SerialPC.println("Bluetooth verbunden.");
  }
  if (!currentBTState && lastBTState) {
    SerialPC.println("Bluetooth getrennt.");
  }
  lastBTState = currentBTState;


  if (SerialBT.available()) {
    String s = SerialBT.readStringUntil('\n');
    digitalWrite(LED, !digitalRead(LED));

    for (int i = 0; i < 4; i++) {

      if (s == "LED" + String(i) + "_ein") {
        digitalWrite(leds[i], HIGH);
      }

      if (s == "LED" + String(i) + "_aus") {
        digitalWrite(leds[i], LOW);
      }

      if (s.equals("Alle_ein")) {
        for (int i = 0; i < 4; i++) {
          digitalWrite(leds[i], HIGH);
        }
      }

      if (s.equals("Alle_aus")) {
        for (int i = 0; i < 4; i++) {
          digitalWrite(leds[i], LOW);
        }
      }
    }
  }
}

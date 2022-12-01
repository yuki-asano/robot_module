#include "Pololu299x.h"

#define BAUD 57600
#define PIN_PWM 3
#define PIN_DIR 4


Pololu299x P(PIN_PWM, PIN_DIR);

void setup() {
  Serial.begin(BAUD);
  P.initialize();
  delay(500);
}

void loop() {
  Serial.println("---START LOOP---");

  P.forwardDrive(20, 1000);
  delay(500);

  P.backwardDrive(20, 1000);
  delay(500);

  Serial.println("");
}

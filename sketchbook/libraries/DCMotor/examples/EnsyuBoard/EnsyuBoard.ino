#include "EnsyuBoard.h"

#define BAUD 57600
#define PIN_PWM 3
#define PIN_IN1 9
#define PIN_IN2 10


EnsyuBoard EB(PIN_PWM, PIN_IN1, PIN_IN2);

void setup() {
  Serial.begin(BAUD);
  EB.initialize();
  delay(500);
}

void loop() {
  Serial.println("---START LOOP---");

  EB.forwardDrive(20, 1000);
  delay(500);

  EB.backwardDrive(20, 1000);
  delay(500);

  Serial.println("");
}

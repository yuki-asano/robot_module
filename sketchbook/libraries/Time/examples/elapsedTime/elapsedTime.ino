#include <Time.h>

#define BAUD 57600

void setup() {
  Serial.begin(BAUD);
  setup_elapsed_time();
}

void loop() {
  unsigned long t = get_elapsed_time();
  delay(500);
  Serial.println(t);
}

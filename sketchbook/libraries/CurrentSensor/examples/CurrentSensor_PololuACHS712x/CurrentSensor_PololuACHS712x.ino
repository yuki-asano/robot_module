#include <CurrentSensor.h>

#define BAUD 115200
#define SENSOR_PIN 0 // analog

float current;

void setup() {
  Serial.begin(BAUD);
  current = 0;
}

void loop() {

  current = getCurrent_ACHS7121(SENSOR_PIN);

  Serial.print("current[A]: ");
  Serial.println(current);

}

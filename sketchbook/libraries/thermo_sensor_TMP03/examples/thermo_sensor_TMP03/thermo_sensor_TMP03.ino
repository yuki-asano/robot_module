#include <thermo_sensor_TMP03.h>

#define SENSOR_PIN 2
#define BAUD 57600

float temp;

void setup() {
  setup_TMP03(SENSOR_PIN);
  Serial.begin(BAUD);
}

void loop() {
  temp = get_temp_TMP03(SENSOR_PIN);
  Serial.println(temp);
}

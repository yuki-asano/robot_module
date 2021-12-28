// Current sensor
// Pololu ACHS-7121: -10 -- 10A, #4030

#define SENSOR_OFFSET_7121 10
#define SENSOR_RANGE_7121 20 // -10 -- 10A

float getCurrent_ACHS7121(int SENSOR_PIN) {
  float current;
  current = analogRead(SENSOR_PIN) / 1023.0 * SENSOR_RANGE_7121 - SENSOR_OFFSET_7121;
  // current / 20 = analogdata / 1023
  // center is 512 (2.5V)

  return current;
}

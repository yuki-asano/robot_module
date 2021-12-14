#include <thermo_sensor_TMP03.h>
#include <Time.h>

#define BAUD 57600
#define SENSOR_PIN 2
#define PWM_PIN 3  // PIN for generating PWM
#define DIR_PIN 4  // PIN for designating rotation direction. HIGH: OUTA -> OUTB, LOW: OUTB -> OUTA.

float temp = 0;
unsigned long elapsed_time = 0;

void setup() {
  Serial.begin(BAUD);
  pinMode(PWM_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  digitalWrite(PWM_PIN, LOW);  // default LOW = disable
  digitalWrite(DIR_PIN, LOW); // default LOW
  delay(500);

  setup_TMP03(SENSOR_PIN);
  setup_elapsed_time();
  Serial.println("# time[ms] temp[degC]");
}

void loop() {
  // motor control
  // forward
  digitalWrite(DIR_PIN, LOW);
  analogWrite(PWM_PIN, 52); // PWM 20% (100% -> 255)
  delay(500);

  analogWrite(PWM_PIN, 0);  // stop
  delay(1000);

  // backward
  digitalWrite(DIR_PIN, HIGH);
  analogWrite(PWM_PIN, 52); // PWM 20% (100% -> 255)
  delay(500);

  analogWrite(PWM_PIN, 0);  // stop
  delay(1000);

  // get temperature
  temp = get_temp_TMP03(SENSOR_PIN);
  elapsed_time = get_elapsed_time();

  Serial.print(elapsed_time);
  Serial.print(" ");
  Serial.println(temp);
}

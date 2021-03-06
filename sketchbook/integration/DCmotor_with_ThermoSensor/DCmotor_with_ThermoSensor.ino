#include <ThermoSensor.h>
#include <Time.h>

#define BAUD 57600
#define SENSOR_PIN 2
#define SENSOR_PIN2 5
#define PWM_PIN 3  // PIN for generating PWM
#define DIR_PIN 4  // PIN for designating rotation direction. HIGH: OUTA -> OUTB, LOW: OUTB -> OUTA.

float temp = 0;
float temp2 = 0;
unsigned long elapsed_time = 0;

void setup() {
  Serial.begin(BAUD);
  pinMode(PWM_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  digitalWrite(PWM_PIN, LOW);  // default LOW = disable
  digitalWrite(DIR_PIN, LOW); // default LOW
  delay(500);

  setup_TMP03(SENSOR_PIN);
  setup_TMP03(SENSOR_PIN2);
  setup_elapsed_time();
  Serial.println("# time[ms] temp[degC] temp2[degC]");
}

void loop() {
  // get temperature
  temp = get_temp_TMP03(SENSOR_PIN);
  temp2 = get_temp_TMP03(SENSOR_PIN2);
  elapsed_time = get_elapsed_time();

  Serial.print(elapsed_time);
  Serial.print(" ");
  Serial.print(temp);
  Serial.print(" ");
  Serial.println(temp2);

  if(temp < 70) {
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
  } else {
    Serial.println("temp is high");
  }
}

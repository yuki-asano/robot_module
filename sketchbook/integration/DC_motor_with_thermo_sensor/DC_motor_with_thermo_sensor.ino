#include <thermo_sensor_TMP03.h>
#include <Time.h>
#include <EncoderPololu.h>
#include <MsTimer2.h>

#define BAUD 57600
#define THERMO_SENSOR_PIN 2
#define PWM_PIN 3  // PIN for generating PWM
#define DIR_PIN 4  // PIN for designating rotation direction. HIGH: OUTA -> OUTB, LOW: OUTB -> OUTA.
#define ENC_PIN1 5
#define ENC_PIN2 6

float temp = 0;
unsigned long elapsed_time = 0;
const float gearRatio = 34.0;
const int encoderCPR = 48;
float motorAngle = 0.0;

EncoderPololu enc(ENC_PIN1, ENC_PIN2, encoderCPR, gearRatio);

void readEnc() {
  enc.readEncoderCount(); // direct use of this in MsTimer2 becomes error.
}

void setup() {
  Serial.begin(BAUD);
  pinMode(PWM_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  digitalWrite(PWM_PIN, LOW);  // default LOW = disable
  digitalWrite(DIR_PIN, LOW); // default LOW
  delay(500);

  setup_TMP03(THERMO_SENSOR_PIN);
  setup_elapsed_time();
  Serial.println("# time[ms] temp[degC]");

  MsTimer2::set(1, readEnc);
  MsTimer2::start();
}

void loop() {
  // get temperature
  temp = get_temp_TMP03(THERMO_SENSOR_PIN);
  elapsed_time = get_elapsed_time();

  // get motor angle
  motorAngle = enc.getMotorAngle();

  Serial.print(elapsed_time);
  Serial.print(" ");
  Serial.println(temp);

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

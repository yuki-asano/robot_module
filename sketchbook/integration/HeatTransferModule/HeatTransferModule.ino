#include <MsTimer2.h>
#include <ThermoSensor.h>
#include <Time.h>
#include <EncoderPololu.h>
#include "../../libraries/DCMotor/examples/EnsyuBoard/EnsyuBoard.h"

#define DEBUG 0
#define BAUD 57600
#define PIN_TMP 2
#define PIN_PWM 5
#define PIN_IN1 9
#define PIN_IN2 10
#define PIN_ENC1 7
#define PIN_ENC2 8

// timer
volatile int tc1; // timer count
volatile int tc2;
volatile int tc3;
volatile bool tc1Flag;
volatile bool tc2Flag;
volatile bool tc3Flag;

// encoder
const float gearRatio = 34.0;
const int encoderCPR = 64;

// sensor data
unsigned long elapsed_time_enc = 0;
unsigned long elapsed_time_temp = 0;
float temp = 0;
float motorAngle = 0.0;

EnsyuBoard EB(PIN_PWM, PIN_IN1, PIN_IN2);
EncoderPololu enc(PIN_ENC1, PIN_ENC2, encoderCPR, gearRatio);


void timerCount()
{
  tc1++;
  tc2++;
  tc3++;

  // tc1: encoder
  if (tc1 >= 1) {
    tc1 = 0;
    tc1Flag = true;
  }
  // tc2: temp sensor
  if (tc2 >= 1000) {
    tc2 = 0;
    tc2Flag = true;
  }
  // tc3: print
  if (tc3 >= 1000) {
    tc3 = 0;
    tc3Flag = true;
  }
}


void setup() {
  Serial.begin(BAUD);
  EB.initialize();
  delay(500);

  setup_TMP03(PIN_TMP);
  setup_elapsed_time();
  Serial.println("# time_temp[ms] temp[degC] time_enc[ms] theta[deg]");

  MsTimer2::set(1, timerCount);
  MsTimer2::start();
}

void loop() {
  // drive motor
  if(temp < 70) {
    EB.forwardDrive(50, 500);
    delay(500);

    EB.backwardDrive(50, 500);
    delay(500);
  } else {
    Serial.println("temp is high");
  }

  // task
  if(tc1Flag) {
    // get angle
    elapsed_time_enc = get_elapsed_time();
    motorAngle = enc.getMotorAngle();
    tc1Flag = false;
  }
  if(tc2Flag) {
    // get temperature
    elapsed_time_temp = get_elapsed_time();
    temp = get_temp_TMP03(PIN_TMP);
    tc2Flag = false;
  }
  if(tc3Flag) {
    // serial print for plot
    Serial.print(elapsed_time_temp, 6);
    Serial.print(" ");
    Serial.print(temp);
    Serial.print(" ");
    Serial.print(elapsed_time_enc, 6);
    Serial.print(" ");
    Serial.println(motorAngle);

    tc3Flag = false;
  }

}

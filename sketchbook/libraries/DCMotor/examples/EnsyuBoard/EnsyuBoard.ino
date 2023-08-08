#define DEBUG 0

#include "EnsyuBoard.h"
#include <MsTimer2.h>
#include <EncoderPololu.h>

#define BAUD 115200
//#define PIN_PWM 3
#define PIN_PWM 5
#define PIN_IN1 9
#define PIN_IN2 10
#define PIN_ENC1 7
#define PIN_ENC2 8

// timer
volatile int tc_1; // timer count
volatile int tc_2;
volatile bool tc_1Flag;
volatile bool tc_2Flag;
unsigned long startTime;
unsigned long endTime;
double cycleTime;

// encoder
//const float gearRatio = 70.0;
//const float gearRatio = 6.3;
const float gearRatio = 34;
const int encoderCPR = 64;

// motor control
float targetAngle = 0.0;
float currAngle = 0.0;
int encCount = 0;
float controlCycle = 0.001;  // [s]
float output;

EnsyuBoard EB(PIN_PWM, PIN_IN1, PIN_IN2, controlCycle);
EncoderPololu enc(PIN_ENC1, PIN_ENC2, encoderCPR, gearRatio);



void timerCount()
{
  tc_1++;
  tc_2++;

  // motor control task: 1ms
  if (tc_1 >= controlCycle*1000) {
    tc_1 = 0;
    tc_1Flag = true;
  }
  // print task: 100ms
  if (tc_2 >= 100) {
    tc_2 = 0;
    tc_2Flag = true;
  }
}


void setup() {
  Serial.begin(BAUD);
  EB.initialize();
  delay(500);

  MsTimer2::set(controlCycle*1000, timerCount);
  MsTimer2::start();
}

void loop() {
  startTime = micros();

  // get angle
  currAngle = enc.getMotorAngle();
  encCount = enc.getEncoderCount();

  // task
  if(tc_1Flag) {
    output = EB.positionControl(targetAngle, currAngle);
    if(output < 0) {
      EB.forwardDrive2(abs(output));
    }
    if(output > 0) {
      EB.backwardDrive2(abs(output));
    }

    tc_1Flag = false;
  }

  if(tc_2Flag) {
    // print
    Serial.println("---START LOOP---");
    Serial.print("targetAngle: ");
    Serial.println(targetAngle);
    Serial.print("currAngle: ");
    Serial.println(currAngle);
    Serial.print("diffAngle[deg]: ");
    Serial.println(EB.diffAngle);
    Serial.print("prevdiffAngle[deg]: ");
    Serial.println(EB.prevdiffAngle);
    Serial.print("integral: ");
    Serial.println(EB.integral);
    Serial.print("output: ");
    Serial.println(EB.output);
    Serial.print("cycleTime[ms]: ");
    Serial.println(cycleTime, 3);

    tc_2Flag = false;
    Serial.println("");
  }

  // serial input
  if(Serial.available() > 0) {
    String inputstr = Serial.readString();
    inputstr.trim();
    targetAngle = inputstr.toFloat();
  }

  endTime = micros();
  cycleTime = (endTime - startTime) / 1000.0;  // [ms]

}

#define DEBUG 0

#include "EnsyuBoard.h"
#include <MsTimer2.h>
//#include <FlexiTimer2.h>
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
//const float p_gain = 0.4;  // 0.5 -> overshoot
//const float d_gain = 0.0;
//const float i_gain = 1.0;
//const float epsilon = 0.5; // [deg]

const float gearRatio = 6.3;
const float p_gain = 0.4;
const float d_gain = 0.0;
const float i_gain = 1.0;
const float epsilon = 1.0; // [deg]


const int encoderCPR = 64;
float motorAngle = 0.0;
float motorAngleNew;
volatile float currAngle = 0.0;  // [rad]
volatile float prevAngle = 0.0;
volatile float prevdiffAngle = 0.0;
volatile float diffAngle = 0.0;
volatile float diffAngle_d = 0.0;
volatile float integral = 0.0;
volatile float output = 0.0;
volatile float targetAngle = 45.0;
volatile float dt = 0.0;


EnsyuBoard EB(PIN_PWM, PIN_IN1, PIN_IN2);
EncoderPololu enc(PIN_ENC1, PIN_ENC2, encoderCPR, gearRatio);

void positionControl() {
  //float targetAngle = targetAngle_deg / 180.0 * 3.14;

  currAngle = enc.getMotorAngle();
  diffAngle = targetAngle - currAngle;
  //dt = (float)cycleTime / 1000.0;  // [s]
  diffAngle_d = (diffAngle - prevdiffAngle) / dt;
  // calc integral if motor angle is not reached to the reference
  if(abs(diffAngle) > epsilon){
    integral += diffAngle * dt;
  }
  //output = p_gain * diffAngle + i_gain * integral + d_gain * diffAngle_d;
  output = p_gain * diffAngle + i_gain * integral;
  //output = p_gain * diffAngle;

  if(output>255)
    output = 255;
  if(output<-255)
    output = -255;

  if(abs(diffAngle) > epsilon){
    if(output < 0) {
      //EB.forwardDrive(abs(output), 1);
      EB.forwardDrive2(abs(output));
    }
    if(output > 0) {
      //EB.backwardDrive(abs(output), 1);
      EB.backwardDrive2(abs(output));
    }
  }

  prevdiffAngle = diffAngle;
  prevAngle = currAngle;
}

void readEnc() {
  motorAngle = enc.getEncoderCount(); // direct use of this in MsTimer2 becomes error.
}

void timerCount()
{
  tc_1++;
  tc_2++;

  if (tc_1 >= 1) {
    tc_1 = 0;
    tc_1Flag = true;
  }

  if (tc_2 >= 100) {
    tc_2 = 0;
    tc_2Flag = true;
  }
}


void setup() {
  Serial.begin(BAUD);
  EB.initialize();
  delay(500);

  MsTimer2::set(1, timerCount);
  MsTimer2::start();
  //FlexiTimer2::set(1, timerCount);
  //FlexiTimer2::start();
}

void loop() {
  startTime = micros();

  //EB.forwardDrive(50, 500);

  // get angle
  //motorAngle = enc.getMotorAngle();
  //diffAngle = positionControl(10);
  //positionControl();
  // task
  if(tc_1Flag) {
    //readEnc();
    positionControl();
    tc_1Flag = false;
  }

  if(tc_2Flag) {
    // print
    Serial.println("---START LOOP---");
    //Serial.print("motorAngle: ");
    //Serial.println(motorAngle);
    Serial.print("currAngle[deg]: ");
    //Serial.println(currAngle/3.14*180);
    Serial.println(currAngle);
    Serial.print("diffAngle[deg]: ");
    //Serial.println(diffAngle/3.14*180);
    Serial.println(diffAngle);
    Serial.print("prevdiffAngle[deg]: ");
    //Serial.println(diffAngle/3.14*180);
    Serial.println(prevdiffAngle);
    Serial.print("integral: ");
    Serial.println(integral);
    Serial.print("output: ");
    Serial.println(output);
    //Serial.print("cycleTime[ms]: ");
    //Serial.println(cycleTime);
    Serial.print("dt[s]: ");
    Serial.println(dt, 6);
    //Serial.print("startTime: ");
    //Serial.println(startTime);
    //Serial.print("endTime: ");
    //Serial.println(endTime);

    tc_2Flag = false;
    Serial.println("");
  }

  //delay(1);
  endTime = micros();
  dt = (endTime - startTime) / (1000.0 * 1000.0);  // [s]

  //EB.backwardDrive(255, 500);
  // get angle
  //motorAngle = enc.getMotorAngle();
  //diffAngle = enc.positionControl(10);

  // print
  //Serial.print("angle: ");
  //Serial.println(motorAngle);
  //Serial.print("diffAngle: ");
  //Serial.println(diffAngle);
  //delay(500);


}

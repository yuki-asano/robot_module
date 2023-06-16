#include <Arduino.h>
#include <MsTimer2.h>
#include <ros.h>
#include <std_msgs/Float32.h>
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

unsigned long startTime = 0;
unsigned long endTime = 0;
unsigned long cycleTime = 0;

// encoder
const float gearRatio = 34.0;
const int encoderCPR = 64;

// sensor data
unsigned long elapsed_time_enc = 0;
unsigned long elapsed_time_temp = 0;
long encCount = 0;
float motorTemp = 0;
float motorAngle = 0.0;

// ros
ros::NodeHandle nh;
std_msgs::Float32 msg_temp;
std_msgs::Float32 msg_angle;
ros::Publisher pub_temp("motorTemp", &msg_temp);
ros::Publisher pub_angle("motorAngle", &msg_angle);

EnsyuBoard EB(PIN_PWM, PIN_IN1, PIN_IN2);
EncoderPololu enc(PIN_ENC1, PIN_ENC2, encoderCPR, gearRatio);


void timerCount()
{
  tc1++;
  tc2++;
  tc3++;

  // tc1: drive motor
  if (tc1 >= 1000) {
    tc1 = 0;
    tc1Flag = true;
  }
  // tc2: temp sensor
  if (tc2 >= 500) {
    tc2 = 0;
    tc2Flag = true;
  }
  // tc3: pubish or print
  if (tc3 >= 1000) {
    tc3 = 0;
    tc3Flag = true;
  }
}


void setup() {
  Serial.begin(BAUD);

  // board setup
  EB.initialize();
  delay(500);
  setup_TMP03(PIN_TMP);
  setup_elapsed_time();

  // ros setup
  nh.getHardware()->setBaud(BAUD);
  nh.initNode();
  nh.advertise(pub_temp);
  nh.advertise(pub_angle);

  // timer setup
  MsTimer2::set(1, timerCount);
  MsTimer2::start();

  Serial.println("# time_temp[ms] temp[degC] time_enc[ms] theta[deg]");
}


void loop() {
  startTime = micros();

  // update encoder and get angle
  elapsed_time_enc = get_elapsed_time();
  motorAngle = enc.getMotorAngle();
  encCount = enc.getEncoderCount();
  msg_angle.data = motorAngle;

  // task
  if(tc1Flag) {
    // drive motor
    if(motorTemp < 70) {
      EB.forwardDrive(50, 500);  // arg: (pwm, delay)
      delay(100)
      EB.backwardDrive(50, 500);
    } else {
      Serial.println("motorTemp is high");
    }

    tc1Flag = false;
  }
  if(tc2Flag) {
    // get temperature
    elapsed_time_temp = get_elapsed_time();
    motorTemp = get_temp_TMP03(PIN_TMP);
    msg_temp.data = motorTemp;

    tc2Flag = false;
  }
  if(tc3Flag) {
    // publish
    pub_angle.publish(&msg_angle);  // angle is not trusted value
    pub_temp.publish(&msg_temp);
    nh.spinOnce();  // period between cycles is neccesary for rosserial communication

    // serial print
    Serial.print(elapsed_time_temp, 6);
    Serial.print(" ");
    Serial.print(motorTemp);
    Serial.print(" ");
    Serial.print(elapsed_time_enc, 6);
    Serial.print(" ");
    Serial.print(motorAngle);
    Serial.print(" (");
    Serial.print("encCount: ");
    Serial.print(encCount);
    Serial.print(", last cycleTime[us]:");
    Serial.print(cycleTime, 6);
    Serial.println(")");

    tc3Flag = false;
  }

  endTime = micros();
  cycleTime = endTime - startTime;
}

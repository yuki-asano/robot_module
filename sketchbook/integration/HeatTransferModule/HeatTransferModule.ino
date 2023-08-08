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
volatile unsigned long elapsed_time_enc = 0;
volatile unsigned long elapsed_time_temp = 0;
volatile long encCount = 0;
volatile float motorTemp = 0;
volatile float motorAngle = 0.0;

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

  // tc1: update temp sensor
  if (tc1 >= 1000) {
    tc1 = 0;
    tc1Flag = true;
  }
  // tc2: serial print
  if (tc2 >= 6000) {
    tc2 = 0;
    tc2Flag = true;
  }
  // // tc3: not used
  // if (tc3 >= 2000) {
  //   tc3 = 0;
  //   tc3Flag = true;
  // }
}

void update_temp() {
  // get temperature
  elapsed_time_temp = get_elapsed_time();
  motorTemp = get_temp_TMP03(PIN_TMP);
}

void update_encoder_and_angle () {
  // update encoder and get angle
  elapsed_time_enc = get_elapsed_time();
  motorAngle = enc.getMotorAngle();
  encCount = enc.getEncoderCount();
  msg_angle.data = motorAngle;
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
  //MsTimer2::set(5000, drive_motor);  // not work. can't use delay in timer?
  MsTimer2::start();

  Serial.println("# time_temp[ms] temp[degC] time_enc[ms] theta[deg]");
}


void loop() {
  startTime = micros();

  // drive motor
  if(motorTemp < 70) {
    EB.forwardDrive(100, 5000);   // duty: 100, delay 5[s]
    //EB.forwardDrive(90, 4000);  // duty: 100 -> jerky motion
    //EB.forwardDrive(77, 2000);  // arg: (duty, delay). duty 30%.
    delay(300);  // over 300ms is necessary for stable drive switch when delay is 5000

    EB.backwardDrive(100, 5000);
    //EB.backwardDrive(90, 4000);
    //EB.backwardDrive(77, 2000);
    delay(300);  // over 300ms is necessary for stable drive switch when delay is 5000
  } else {
    Serial.println("motorTemp is high");
  }

  // task
  if(tc1Flag) {
    update_temp();  // temp value is wrong when this is in timer directry
    tc1Flag = false;
  }
  if(tc2Flag) {
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

    tc2Flag = false;
  }
  // if(tc3Flag) {
  //   tc3Flag = false;
  // }

  // publish
  msg_temp.data = motorTemp;
  pub_temp.publish(&msg_temp);
  nh.spinOnce();  // period between cycles is neccesary for rosserial communication

  endTime = micros();
  cycleTime = endTime - startTime;
}

//d Current sensor
// Pololu ACHS-7121: -10 -- 10A, #4030

#include <Arduino.h>
#include <ros.h>
#include <std_msgs/Float32.h>

#define BAUD 115200
#define ROS_BAUD 115200
#define SENSOR_PIN 0
#define SENSOR_OFFSET_7121 10
#define SENSOR_RANGE_7121 20 // -10 -- 10A

float current;

ros::NodeHandle nh;
std_msgs::Float32 msg;
ros::Publisher pub("current", &msg);

void setup() {
  Serial.begin(BAUD);
  current = 0;

  // setup ros functions
  nh.getHardware()->setBaud(ROS_BAUD);
  nh.initNode();
  nh.advertise(pub);
}

void loop() {

  current = analogRead(SENSOR_PIN) / 1023.0 * SENSOR_RANGE_7121 - SENSOR_OFFSET_7121;
  // current / 20 = analogdata / 1023
  // center is 512 (2.5V)

  msg.data = current;
  pub.publish(&msg);
  nh.spinOnce();

  Serial.print("current[A]: ");
  Serial.println(current);

}

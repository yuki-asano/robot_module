// Current sensor
// Pololu ACHS-7121: -10 -- 10A, #4030

#include <Arduino.h>
#include <ros.h>
#include <std_msgs/Float32.h>
#include <CurrentSensor.h>

#define BAUD 115200
#define ROS_BAUD 115200
#define SENSOR_PIN 0

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

  current = getCurrent_ACHS7121(SENSOR_PIN);

  msg.data = current;
  pub.publish(&msg);
  nh.spinOnce();

  Serial.print("current[A]: ");
  Serial.println(current);

}

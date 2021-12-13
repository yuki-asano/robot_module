#include <Arduino.h>
#include <ros.h>
#include <std_msgs/Float32.h>

#define BAUD 115200
#define ROS_BAUD 115200

volatile int pulse_num;
float flow_rate;
unsigned long time_current;
unsigned long time_loop;
unsigned long time_sampling_ms;
unsigned long time_sampling_s;

ros::NodeHandle nh;
std_msgs::Float32 msg;
ros::Publisher pub("flow_rate", &msg);

void setup() {
  Serial.begin(BAUD);
  attachInterrupt(0, pulse_count, RISING); // 1st arg: 0 -> 2pin, 1 -> 3pin.
  pulse_num = 0;
  flow_rate = 0; // [l/min]
  time_current = millis();
  time_loop = time_current;
  time_sampling_ms = 1000; // 1[s]
  time_sampling_s = time_sampling_ms / 1000;

  // setup ros functions
  nh.getHardware()->setBaud(ROS_BAUD);
  nh.initNode();
  nh.advertise(pub);
}

void pulse_count() {
  pulse_num++;
}

void loop() {
  time_current = millis();

  if(time_current >= (time_loop + time_sampling_ms))
  {
      time_loop = time_current;
      flow_rate = pulse_num / time_sampling_s / 7.5; // Freq = 7.5 * Q (L/min)
      
      msg.data = flow_rate;
      pub.publish(&msg);
      nh.spinOnce();

      Serial.print("flow_rate[l/m]: ");
      Serial.println(flow_rate);
      pulse_num = 0; // reset counter
  }
}

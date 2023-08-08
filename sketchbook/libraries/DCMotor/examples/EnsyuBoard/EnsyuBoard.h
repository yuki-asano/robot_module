// Motor driver IC: Toshiba TB67H450FNG
#include "DCMotor.h"

#ifndef DEBUG
#define DEBUG 0
#endif


class EnsyuBoard : public DCMotor {
 public:
  uint8_t pin_in1;
  uint8_t pin_in2;
  float currAngle = 0.0;  // [rad]
  float prevAngle = 0.0;
  float prevdiffAngle = 0.0;
  float diffAngle = 0.0;
  float diffAngle_d = 0.0;
  float integral = 0.0;
  float output = 0.0;
  float targetAngle = 0.0;
  float dt;

  const float p_gain = 3.0;
  const float d_gain = 2.0;
  const float i_gain = 0.0;
  const float epsilon = 1.0; // [deg]

 EnsyuBoard(uint8_t _pin_pwm, uint8_t _pin_in1, uint8_t _pin_in2, float controlCycle) :
  DCMotor(_pin_pwm)
  {
    pin_in1 = _pin_in1;
    pin_in2 = _pin_in2;
    dt = controlCycle;
  }

  ~EnsyuBoard() {};

  void initialize() {
    pinMode(pin_in1, OUTPUT);
    pinMode(pin_in2, OUTPUT);

    stopDrive();
    PWMdutyChange(0);
  }

  // IN1 HIGH and IN2 LOW -> forward
  void forwardDrive(uint8_t pwm_duty, unsigned int duration) {
    if(DEBUG) {
      Serial.print("Driving forward ");
      Serial.print(duration);
      Serial.println("[ms]");
    }

    PWMdutyChange(pwm_duty);
    digitalWrite(pin_in1, HIGH);
    digitalWrite(pin_in2, LOW);

    delay(duration);
    stopDrive();
  }

  void forwardDrive2(uint8_t pwm_duty) {
    if(DEBUG) {
      Serial.print("Driving forward ");
    }

    PWMdutyChange(pwm_duty);
    digitalWrite(pin_in1, HIGH);
    digitalWrite(pin_in2, LOW);
  }

  // IN1 LOW and IN2 HIGH -> backward
  void backwardDrive(uint8_t pwm_duty, unsigned int duration) {
    if(DEBUG) {
      Serial.print("Driving backward ");
      Serial.print(duration);
      Serial.println("[ms]");
    }

    PWMdutyChange(pwm_duty);
    digitalWrite(pin_in1, LOW);
    digitalWrite(pin_in2, HIGH);

    delay(duration);
    stopDrive();
  }

  void backwardDrive2(uint8_t pwm_duty) {
    if(DEBUG) {
      Serial.print("Driving backward ");
    }

    PWMdutyChange(pwm_duty);
    digitalWrite(pin_in1, LOW);
    digitalWrite(pin_in2, HIGH);
  }

  // IN1 LOW and IN2 LOW -> Stop(Standby)
  void stopDrive() {
    if(DEBUG) {
      Serial.println("Stop");
    }

    digitalWrite(pin_in1, LOW);
    digitalWrite(pin_in2, LOW);
  }

  // IN1 HIGH and IN2 HIGH -> Brake
  // brakeDrive brings sudden stop
  void brakeDrive() {
    if(DEBUG) {
      Serial.println("Brake");
    }

    digitalWrite(pin_in1, HIGH);
    digitalWrite(pin_in2, HIGH);
  }


  float positionControl(float tAngle, float cAngle) {
    targetAngle = tAngle;
    currAngle = cAngle;
    diffAngle = targetAngle - currAngle;
    diffAngle_d = (diffAngle - prevdiffAngle) / dt;
    // calc integral if motor angle is not reached to the reference
    if(abs(diffAngle) > epsilon){
      integral += diffAngle * dt;
    }
    //output = p_gain * diffAngle + i_gain * integral + d_gain * diffAngle_d;
    output = p_gain * diffAngle + i_gain * integral;
    //output = p_gain * diffAngle;

    if(output>100)
      output = 100;
    if(output<-100)
      output = -100;

    if(abs(diffAngle) < epsilon){
      output = 0.0;
    }

    prevdiffAngle = diffAngle;
    prevAngle = currAngle;

    return output;
  }

};

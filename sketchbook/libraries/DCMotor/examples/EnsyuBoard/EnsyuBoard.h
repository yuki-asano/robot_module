// Motor driver IC: Toshiba TB67H450FNG
#include "DCMotor.h"

#ifndef DEBUG
#define DEBUG 1
#endif


class EnsyuBoard : public DCMotor {
 public:
  uint8_t pin_in1;
  uint8_t pin_in2;

 EnsyuBoard(uint8_t _pin_pwm, uint8_t _pin_in1, uint8_t _pin_in2) :
  DCMotor(_pin_pwm)
  {
    pin_in1 = _pin_in1;
    pin_in2 = _pin_in2;
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

};

// Pololu G2 High-Power Motor Driver 24v13 #2992
// Pololu G2 High-Power Motor Driver 24v21 #2995
#include "DCMotor.h"

#ifndef DEBUG
#define DEBUG 0
#endif


class Pololu299x : public DCMotor {
 public:
  uint8_t pin_pwm;
  uint8_t pin_dir;  // designating rotation direction

 Pololu299x(uint8_t _pin_pwm, uint8_t _pin_dir):
  DCMotor(_pin_pwm)
  {
    pin_pwm = _pin_pwm;
    pin_dir = _pin_dir;
  }

  ~Pololu299x() {};


  void initialize() {
    pinMode(pin_pwm, OUTPUT);
    pinMode(pin_dir, OUTPUT);
    digitalWrite(pin_pwm, LOW);  // default LOW = disable
    digitalWrite(pin_dir, LOW);  // default LOW
  }

  //pin_dir:
  // HIGH: forward drive (OUTA -> OUTB)
  // LOW : backward drive(OUTB -> OUTA)
  void forwardDrive(uint8_t pwm_duty, unsigned int duration) {
    if(DEBUG) {
      Serial.println("Driving forward");
      Serial.print(duration);
      Serial.println("[ms]");
    }

    digitalWrite(pin_dir, HIGH);  // change direction
    PWMdutyChange(pwm_duty);

    delay(duration);
    brakeDrive();
  }

  void backwardDrive(uint8_t pwm_duty, unsigned int duration) {
    if(DEBUG) {
      Serial.println("Driving backward");
      Serial.print(duration);
      Serial.println("[ms]");
    }

    digitalWrite(pin_dir, LOW);  // change direction
    PWMdutyChange(pwm_duty);

    delay(duration);
    brakeDrive();
  }

  void brakeDrive() {
    if(DEBUG) {
      Serial.println("Brake");
    }

    digitalWrite(pin_pwm, LOW);
  }

};

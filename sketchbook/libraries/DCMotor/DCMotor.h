// General class for DC motor control with Arduino

class DCMotor {
 public:
  uint8_t pwm;
  uint8_t pin_pwm;

  DCMotor(uint8_t _pin_pwm) {
    pwm = 0;
    pin_pwm = _pin_pwm;
  }

  ~DCMotor() {};

  void PWMdutyChange(uint8_t _duty) {
    float duty = _duty;
    if (duty > 100) {
      duty = 100;
    }
    pwm = 255 * duty / 100;
    analogWrite(pin_pwm, pwm);

    Serial.print("Change PWM duty to ");
    Serial.print(int(duty));
    Serial.print("% (value:");
    Serial.print(pwm);
    Serial.println(")");
  }

};

#include <EncoderPololu.h>
#include <MsTimer2.h>

#define BAUD 115200
#define ENC_PIN1 2
#define ENC_PIN2 3

const float gearRatio = 4.4;
const int encoderCPR = 48;
float motorAngle = 0.0;
float motorAngleNew;
String str;

EncoderPololu enc(ENC_PIN1, ENC_PIN2, encoderCPR, gearRatio);

void readEnc() {
  enc.readEncoderCount(); // direct use of this in MsTimer2 becomes error.
}

void setup() {
  Serial.begin(BAUD);
  MsTimer2::set(1, readEnc);
  MsTimer2::start();
}

void loop() {

  motorAngle = enc.getMotorAngle();
  Serial.println(motorAngle);

  if(Serial.available() > 0) {
    str = Serial.readStringUntil('\n');
    if(str == "r") {
      Serial.println("reset angle");
      enc.resetEncoderCount();
    } else if (str.toFloat()) {
      Serial.println("update angle");
      motorAngleNew = str.toFloat();
      enc.overwriteMotorAngle(motorAngleNew);
    } else {
      Serial.println("invalid command");
    }
  }

}

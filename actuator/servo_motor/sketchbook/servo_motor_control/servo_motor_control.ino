#include <Servo.h>

#define BAUD 9600    // シリアル通信のボーレート
#define MOTOR_PIN 11 // サーボモータに接続するデジタルポート

Servo myServo;
int angle = 0;

void setup() {
  myServo.attach(MOTOR_PIN);
  Serial.begin(BAUD);
}

void loop() {
  myServo.write(angle);

  Serial.print("angle: ");
  Serial.println(angle);

  delay(500);

  if(angle < 180) {
    angle += 45;
  } else {
    angle = 0;
  }
}
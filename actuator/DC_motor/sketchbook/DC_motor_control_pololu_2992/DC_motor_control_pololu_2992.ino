// DC motor control
// Pololu G2 High-Power Motor Driver 24v13
// Pololu item #2992  
//
#define BAUD 57600 // Baud rate
#define PWM_PIN 3  // PIN for generating PWM
#define DIR_PIN 4  // PIN for designating rotation direction. HIGH: OUTA -> OUTB, LOW: OUTB -> OUTA.

void setup() {

  Serial.begin(BAUD);      // initialize serial communication
  pinMode(PWM_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  digitalWrite(PWM_PIN, LOW);  // default LOW = disable
  digitalWrite(DIR_PIN, LOW); // default LOW
  delay(500);
}

void loop() {

  // Forward
  Serial.println("Forward");
  digitalWrite(DIR_PIN, LOW);
  analogWrite(PWM_PIN, 52); // PWM 20% (100% -> 255)
  delay(500);

  analogWrite(PWM_PIN, 0);  // stop
  delay(1000);

  // backward 
  Serial.println("Backward");
  digitalWrite(DIR_PIN, HIGH);
  analogWrite(PWM_PIN, 52); // PWM 20% (100% -> 255)
  delay(500);

  analogWrite(PWM_PIN, 0);  // stop
  delay(1000);

}

#define BAUD 57600
#define SENSOR_PIN 2
#define PWM_PIN 3  // PIN for generating PWM
#define DIR_PIN 4  // PIN for designating rotation direction. HIGH: OUTA -> OUTB, LOW: OUTB -> OUTA.
#define DEBUG 0

unsigned long time_high = 0;
unsigned long time_high2 = 0;
unsigned long time_low = 0;
unsigned long time_T1 = 0;
unsigned long time_T2 = 0;
unsigned long time_period = 0;
unsigned long time_begin = 0;
unsigned long time_now = 0;
unsigned long time_total = 0;
float temp = 0;

void setup() {
  Serial.begin(BAUD);
  pinMode(SENSOR_PIN, INPUT_PULLUP);
  pinMode(PWM_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  digitalWrite(PWM_PIN, LOW);  // default LOW = disable
  digitalWrite(DIR_PIN, LOW); // default LOW
  delay(500);
}

void loop() {  
  time_begin = millis();
  Serial.println("# time[ms] temp[degC]");
  //
  for (;;){
    // get rise and fall time of a pulse
    while (digitalRead(SENSOR_PIN) == HIGH) {}
    while (digitalRead(SENSOR_PIN) == LOW) {}
    time_high = micros();
    while (digitalRead(SENSOR_PIN) == HIGH) {}
    time_low = micros();
    while (digitalRead(SENSOR_PIN) == LOW) {}
    time_high2 = micros();
    // time period and temperature calculation
    time_T1 = time_low - time_high;
    time_T2 = time_high2 - time_low;
    time_period = time_T1 + time_T2;
    time_now = millis();
    time_total = time_now - time_begin;
    temp = 235 - (400 * (float)time_T1 / (float)time_T2); // see datasheet of TMP03
    delayMicroseconds(100);
    //
    if(DEBUG){
      Serial.print("time_high = ");
      Serial.println(time_high);
      Serial.print("time_low = ");
      Serial.println(time_low);
      Serial.print("time_high2 = ");
      Serial.println(time_high2);
      Serial.print("time_T1 = ");
      Serial.println(time_T1);
      Serial.print("time_T2 = ");
      Serial.println(time_T2);
      Serial.print("time_period = ");
      Serial.println(time_period);
      Serial.print("temp =");
      Serial.println(temp);
      Serial.println("");
    }
    Serial.print(time_total);
    Serial.print(" ");
    Serial.println(temp);
  
    // Forward
    digitalWrite(DIR_PIN, LOW);
    analogWrite(PWM_PIN, 52); // PWM 20% (100% -> 255)
    delay(500);

    analogWrite(PWM_PIN, 0);  // stop
    delay(1000);

    // backward 
    digitalWrite(DIR_PIN, HIGH);
    analogWrite(PWM_PIN, 52); // PWM 20% (100% -> 255)
    delay(500);

    analogWrite(PWM_PIN, 0);  // stop
    delay(1000);
    }   
}

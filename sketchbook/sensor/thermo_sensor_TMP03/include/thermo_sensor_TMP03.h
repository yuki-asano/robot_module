#define DEBUG 0

void setup_TMP03(int SENSOR_PIN) {
  pinMode(SENSOR_PIN, INPUT_PULLUP);
}

float get_temp_TMP03(int SENSOR_PIN) {
  unsigned long time_high = 0;
  unsigned long time_high2 = 0;
  unsigned long time_low = 0;
  unsigned long time_T1 = 0;
  unsigned long time_T2 = 0;
  unsigned long time_period = 0;
  float temp = 0;

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

  return temp;
}

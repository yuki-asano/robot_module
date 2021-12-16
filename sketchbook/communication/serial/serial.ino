int i = 0;

void setup() {
    Serial.begin(57600);
    while (!Serial) {
        ;
    }
}

void loop() {
  Serial.println(i);
  Serial.println("Serial print");
  delay(500);
  i++;
}

unsigned long startTime;
unsigned long elapsedTime;

void setup_elapsed_time() {
    startTime = millis();
}

void calc_elapsed_time() {
  elapsedTime = millis() - startTime;
}

unsigned long startTime_elapsed;

void setup_elapsed_time() {
  startTime_elapsed = millis();
}

unsigned long get_elapsed_time() {
  unsigned long elapsedTime;
  return elapsedTime = millis() - startTime_elapsed;
}

unsigned long startTime;

void setup_elapsed_time() {
  startTime = millis();
}

unsigned long get_elapsed_time() {
  unsigned long elapsedTime;
  return elapsedTime = millis() - startTime;
}

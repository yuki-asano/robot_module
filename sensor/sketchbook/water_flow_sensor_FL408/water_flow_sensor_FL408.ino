#define BAUD 115200

volatile int pulse_num;
double flow_rate;
unsigned long time_current;
unsigned long time_loop;
unsigned long time_sampling;

void setup() {
  //pinMode(SENSOR_PIN, INPUT);
  Serial.begin(BAUD);
  attachInterrupt(0, pulse_count, RISING); // 1st arg: 0 -> 2pin, 1 -> 3pin.
  pulse_num = 0;
  flow_rate = 0; // [l/min]
  time_current = millis();
  time_loop = time_current;
  time_sampling = 1000; // 1[s]
}

void pulse_count() {
  pulse_num++;
}

void loop() {
  time_current = millis();

  if(time_current >= (time_loop + time_sampling))
  {
      time_loop = time_current;
      flow_rate = pulse_num / 7.5; // F = 7.5 * Q (L/min)
      Serial.print("flow_rate[l/m]: ");
      Serial.println(flow_rate);
      pulse_num = 0; // reset counter
  } 
}

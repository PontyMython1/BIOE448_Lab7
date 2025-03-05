#include "thingProperties.h"
int sensor_pin = 0;
int pulse_signal = 0;
int upper_threshold = 950; //or your threshold of choice
int lower_threshold = 200; //or your threshold of choice
int counter = 0;
//float BPM = 0.0;
bool ignore = false;
bool first_pulse_detected = false;
unsigned long first_pulse_time = 0;
unsigned long second_pulse_time = 0;
unsigned long pulse_period = 0;

void setup() {
  Serial.begin(9600);
  delay(1500);
  initProperties();
  //Connect to cloud and get info/errors
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
  //Wait for cloud connection
  while (ArduinoCloud.connected() != 1) {
      ArduinoCloud.update();
      delay(500);
  }
}

void loop() {
  pulse_signal = analogRead(sensor_pin);
  Serial.println(pulse_signal);
  delay(100);
  if (pulse_signal > upper_threshold && any_peak_detected == false) {
    any_peak_detected = true;
    if (first_peak_detected == false) {
           first_pulse_time = millis();
           first_peak_detected = true;
        } else {
           second_pulse_time = millis();
           pulse_period = second_pulse_time - first_pulse_time;
           first_peak_detected = false;
           any_peak_detected = false;
           BPM = (60000/(pulse_period)); 
           //Serial.println(BPM);
        }
    
}
  if (pulse_signal < lower_threshold) {
    any_peak_detected = false;
        }

  counter++;
  if (counter > 200){
    ArduinoCloud.update();
    Serial.println(BPM);
    counter = 0;
}
}

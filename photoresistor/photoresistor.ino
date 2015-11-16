
//#include "Axis.h"

const int PHOTO_PIN = A3;
const int LED_PIN   = 9;//the number of the LED pin

const int NUM_SAMPLES = 20;
const int THRESHOLD = 40;


void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);
}
 
void loop() {
  
  int x = readAnalogPin(PHOTO_PIN);
  Serial.print("x: ");
  Serial.println(x);
  
  if(x < THRESHOLD) {
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }
  delay(1000);
  
}


int readAnalogPin(int pin) {
  long reading = 0;
  analogRead(pin);
  delay(1);
  for (int i = 0; i < NUM_SAMPLES; i++) {
    reading += analogRead(pin);
    delay(1);
  }
  return reading/NUM_SAMPLES;
}


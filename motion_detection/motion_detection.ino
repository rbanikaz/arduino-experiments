
//#include "Axis.h"

const int LED_PIN = 9;//the number of the LED pin

const int NUM_SAMPLES = 10;
const int ACCEL_THRESHOLD = 4;

const int IDLE_TIME_MILLIS = 1000 * 5;

const int xInput = A0;
int xVal = 0;
unsigned long xTs = millis();

const int yInput = A1;
int yVal = 0;
unsigned long yTs = millis();

const int zInput = A2;
int zVal = 0;
unsigned long zTs = millis();

void setup() {
  pinMode(LED_PIN, OUTPUT);//initialize the digital pin as an output
  Serial.begin(9600);
}
 
void loop() {
  if(detectIfMoving()) {
    digitalWrite(LED_PIN, HIGH);//turn the LED on
  } else {
    digitalWrite(LED_PIN, LOW);//turn the LED on
  }
  
  delay(1000);
  
}

boolean detectIfMoving() {
  Serial.println("Checking xAxis:");
  boolean xIsMoving = detectIfMoving(xInput, &xVal, &xTs);
  Serial.println();
  
  Serial.println("Checking yAxis:");  
  boolean yIsMoving = detectIfMoving(yInput, &yVal, &yTs);
  Serial.println();
  
  Serial.println("Checking zAxis:");  
  boolean zIsMoving = detectIfMoving(zInput, &zVal, &zTs);
  Serial.println();
  
  return xIsMoving || yIsMoving || zIsMoving;
}

boolean detectIfMoving(int axisPin, int* currentValue, unsigned long* timestamp) {
  int newValue = readAnalogPin(axisPin);
  unsigned long now = millis();
  unsigned long elapsedTime = now - *timestamp;
  boolean isMoving = true;
  
  if(abs(newValue - *currentValue) > ACCEL_THRESHOLD) {
    *timestamp = now;
  } else if(elapsedTime > IDLE_TIME_MILLIS) {
    isMoving = false;
  }
  
  printAxisData(newValue, *currentValue, elapsedTime, isMoving);
  
  *currentValue = newValue;
  
  return isMoving;
}

void printAxisData(int newValue, int currentValue, unsigned long elapsedTime, boolean isMoving) {
  Serial.print("newValue: ");
  Serial.print(newValue);
  Serial.print("; currentValue: ");
  Serial.print(currentValue);
  Serial.print("; elapsedTime: ");
  Serial.print(elapsedTime);
  Serial.print("; isMoving: ");
  Serial.print(isMoving);
  Serial.println();
 
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



 


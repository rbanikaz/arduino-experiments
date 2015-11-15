
//#include "Axis.h"

const int LED_PIN = 9;//the number of the LED pin

const int NUM_SAMPLES = 10;
const int THRESHOLD = 4;
const int NUM_POINTS = 3;

int index = 0;

const int xInput = A0;
int xPoints[] = { 0, 0, 0 };

const int yInput = A1;
int yPoints[] = { 0, 0, 0 };

const int zInput = A2;
int zPoints[] = { 0, 0, 0 };

void setup() {
  pinMode(LED_PIN, OUTPUT);//initialize the digital pin as an output
  Serial.begin(9600);
}
 
void loop() {
  
  Serial.println("Checking xAxis:");
  boolean xIsMoving = detectIfMoving(xInput, xPoints);
  Serial.println();
  
  Serial.println("Checking yAxis:");  
  boolean yIsMoving = detectIfMoving(yInput, yPoints);
  Serial.println();
  
  Serial.println("Checking zAxis:");  
  boolean zIsMoving = detectIfMoving(zInput, zPoints);
  Serial.println();

  if(xIsMoving || yIsMoving || zIsMoving) {
    digitalWrite(LED_PIN,HIGH);//turn the LED on
  } else {
    digitalWrite(LED_PIN,LOW);//turn the LED on
  }
  
  index++;
  index %= NUM_POINTS;
  
  delay(1000);
  
}


boolean detectIfMoving(int axisPin, int* dataPoints) {
  int newDataPoint = readAxis(axisPin);
  Serial.print("newDataPoint: ");
  Serial.print(newDataPoint);
  Serial.print("; [");
  boolean isMoving = false;
  
  for(int i = 0; i < NUM_POINTS; i++) {
    if(dataPoints[i] > 0) {
      Serial.print(dataPoints[i]);
      Serial.print(",");
      if(abs(newDataPoint - dataPoints[i]) > THRESHOLD) {
        isMoving = true;
      }
    }
  }
  Serial.print("]; isMoving: ");
  Serial.print(isMoving);
  Serial.println();
  
  dataPoints[index] = newDataPoint;
  
  return isMoving;
}

int readAxis(int axisPin) {
  long reading = 0;
  analogRead(axisPin);
  delay(1);
  for (int i = 0; i < NUM_SAMPLES; i++) {
    reading += analogRead(axisPin);
    delay(1);
  }
  return reading/NUM_SAMPLES;
}



 


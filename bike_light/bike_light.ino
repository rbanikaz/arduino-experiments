


const int PHOTO_PIN = A3;
const int LED_PIN = 9;//the number of the LED pin

const int NUM_SAMPLES = 100;
const int ACCEL_THRESHOLD = 4;
const int PHOTO_THRESHOLD = 40;

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

/***** API *****/

void setup() {
  pinMode(LED_PIN, OUTPUT);//initialize the digital pin as an output
  Serial.begin(9600);
}
 
 
void loop() {
  boolean isMoving = detectIfMoving();
  boolean isDarkOut = detectIfDarkOut();
  
  if(isMoving && isDarkOut) {
    turnOnLamp();
  } else {
    turnOffLamp();
  }
  
  delay(1000);
  Serial.println();
}


/***** Subroutines *****/

void turnOnLamp() {  Serial.println("turning on lamp"); digitalWrite(LED_PIN, HIGH); }
void turnOffLamp() { Serial.println("turning off lamp"); digitalWrite(LED_PIN, LOW); }

boolean detectIfDarkOut() {
  int photoLevel =  readAnalogPin(PHOTO_PIN);
  Serial.print("photoLevel: ");
  Serial.println(photoLevel);
  
  if(photoLevel < PHOTO_THRESHOLD) {
    Serial.println("dark");
    return true;
  }
  Serial.println("not dark");
  return false;

}

boolean detectIfMoving() {
 
  boolean xIsMoving = detectIfMoving(xInput, &xVal, &xTs);
  boolean yIsMoving = detectIfMoving(yInput, &yVal, &yTs);
  boolean zIsMoving = detectIfMoving(zInput, &zVal, &zTs);
 
 
  Serial.print("xIsMoving: ");
  Serial.print(xIsMoving);
  Serial.print("; yIsMoving: ");
  Serial.print(yIsMoving);
  Serial.print("; zIsMoving: ");
  Serial.print(zIsMoving);
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
  
  //printAxisData(newValue, *currentValue, elapsedTime, isMoving);
  
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



 


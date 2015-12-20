
const boolean DEBUG = true;

const int LIGHT_SENSOR_ENABLE_PIN = 7;
const int LED_PIN = 9;

const int NUM_SAMPLES = 10;
const int ACCEL_THRESHOLD = 4;
const int LIGHT_THRESHOLD = 20;

const int IDLE_TIME_MILLIS = 1000 * 5;

char* X_PIN_NAME = "x";
const int X_PIN = A0;
int xVal = 0;
unsigned long xTs = millis();

char* Y_PIN_NAME = "y";
const int Y_PIN = A1;
int yVal = 0;
unsigned long yTs = millis();

char* Z_PIN_NAME = "z";
const int Z_PIN = A2;
int zVal = 0;
unsigned long zTs = millis();

const int LIGHT_PIN = A3;
boolean currLightReading = false;
boolean currLightStatus = false;
unsigned long lightTs = millis();



/***** API *****/

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(LIGHT_SENSOR_ENABLE_PIN, INPUT);
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
  boolean isLightSensorEnabled = digitalRead(LIGHT_SENSOR_ENABLE_PIN) == HIGH;
  
  if(isLightSensorEnabled) {
    boolean isDarkOut = detectLightStatus();
    
    if(isDarkOut) {
      Serial.println("its dark out");
      return true;
    }
    Serial.println("not dark out");
    return false;
  }
  
  Serial.println("light detection disabled");
  return true;
}



boolean detectLightStatus() {
  int lightValue = readAnalogPin(LIGHT_PIN);
  
  Serial.print("Read light value of ");
  Serial.println(lightValue);
  
  boolean newLightReading = lightValue <= LIGHT_THRESHOLD;
  
  if(newLightReading != currLightReading) {
    lightTs = millis();
  } else {
    unsigned long now = millis();
    unsigned long elapsedTime = now - lightTs;
    
    if(elapsedTime > IDLE_TIME_MILLIS) {
      Serial.println("updating light status");
      currLightStatus = currLightReading;
    }
  }
  
  currLightReading = newLightReading;
  
  return currLightStatus;
}


boolean detectIfMoving() {
 
  boolean xIsMoving = detectAccelStatus(X_PIN_NAME, X_PIN, ACCEL_THRESHOLD, IDLE_TIME_MILLIS, &xVal, &xTs);
  boolean yIsMoving = detectAccelStatus(Y_PIN_NAME, Y_PIN, ACCEL_THRESHOLD, IDLE_TIME_MILLIS, &yVal, &yTs);
  boolean zIsMoving = detectAccelStatus(Z_PIN_NAME, Z_PIN, ACCEL_THRESHOLD, IDLE_TIME_MILLIS, &zVal, &zTs);
 
 
  Serial.print("xIsMoving: ");
  Serial.print(xIsMoving);
  Serial.print("; yIsMoving: ");
  Serial.print(yIsMoving);
  Serial.print("; zIsMoving: ");
  Serial.print(zIsMoving);
  Serial.println();
  
  return xIsMoving || yIsMoving || zIsMoving;
}

boolean detectAccelStatus(char* pinName, int pin, int threshold, unsigned long timeOut, int* currentValue, unsigned long* timestamp) {
  int newValue = readAnalogPin(pin);
  unsigned long now = millis();
  unsigned long elapsedTime = now - *timestamp;
  boolean pinStatus = true;
  
  if(abs(newValue - *currentValue) > threshold) {
    *timestamp = now;
  } else if(elapsedTime > timeOut) {
    pinStatus = false;
  }
  
  printAccelStatus(pinName, newValue, *currentValue, elapsedTime, pinStatus);
  
  *currentValue = newValue;
  
  return pinStatus;
}

void printAccelStatus(char* pinName, int newValue, int currentValue, unsigned long elapsedTime, boolean pinStatus) {
  
  Serial.print(pinName);
  Serial.print(": newValue: ");
  Serial.print(newValue);
  Serial.print("; currentValue: ");
  Serial.print(currentValue);
  Serial.print("; elapsedTime: ");
  Serial.print(elapsedTime);
  Serial.print("; pinStatus: ");
  Serial.print(pinStatus);
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

 


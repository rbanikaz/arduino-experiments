
const boolean DEBUG = true;

const int LIGHT_SENSOR_ENABLE_PIN = 7;
const int LED_PIN = 9;

const int NUM_SAMPLES = 10;
const int LIGHT_THRESHOLD = 20;

const int IDLE_TIME_MILLIS = 1000 * 5;

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
  boolean isDarkOut = detectIfDarkOut();
  
  if(isDarkOut) {
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
  boolean isPhotoEnabled = digitalRead(LIGHT_SENSOR_ENABLE_PIN) == HIGH;
  
  if(isPhotoEnabled) {
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
    Serial.println("Reading diff from current status, reseting timestamp");
    lightTs = millis();
  } else {
    Serial.println("Checking elapsedTime");
    unsigned long now = millis();
    unsigned long elapsedTime = now - lightTs;
    
    Serial.print("Status has been same for ");
    Serial.print(elapsedTime);
    Serial.println(" milliseconds");
    
    if(elapsedTime > IDLE_TIME_MILLIS) {
      Serial.println("Greater than timeout, must be consistent");
      currLightStatus = currLightReading;
    }
  }
  
  currLightReading = newLightReading;
  
  return currLightStatus;
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



 


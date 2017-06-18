class StatisticsData {
 float distance;
 float speedBike;
 float timeInTrip;
 int clories;
 int countDangerousSituation;
 public:
    void setDistance (float distanceCurrent){
      distance = distanceCurrent;
    }
    float getDistance() {
      return distance;
    }
    void setSpeedBike (float speedB){
      speedBike = speedB;
    }
    float getSpeedBike() {
      return speedBike;
    }
    void setTimeTrip (float timeTrip){
      timeInTrip = timeTrip;
    }
    float getTimeTrip() {
      return timeInTrip;
    }
    void setCalories (int calor){
      clories = calor;
    }
    int getCalories() {
      return clories;
    }
    void setDangerousSituation (int dangerous){
      countDangerousSituation = dangerous;
    }
    int getDangerousSituation() {
      return countDangerousSituation;
    }
};

#include <SoftwareSerial.h>

int touchSensorPin = 6; 
int moveDetectorPin = 7; 
int ledPinTouch = 8;
int ledPinMove = 9;

int val = 0;
int countTouch = 1;
float countTime = 0;
int countDangerousSituation = 1;
float meedleLenth = 3.89;
float meedleWeight = 70;
int countDelay = 2;
int countMetrInKm = 1000;
int countMillisInSec = 1000;
int oneDelay = 1000;

int minSecsBetweenSend = 60;
long lastSend = -minSecsBetweenSend * 1000;
bool checked;

long oldTimestamp = millis();

StatisticsData data;

#define rxPin 2
#define txPin 3  
  
SoftwareSerial blueTooth(rxPin, txPin);

void setup(){
  pinMode(moveDetectorPin, INPUT);
  pinMode(ledPinMove, OUTPUT);
  pinMode(touchSensorPin, INPUT);
  pinMode(ledPinTouch, OUTPUT);
  Serial.begin(9600);
  blueTooth.begin(9600);  

}

void loop(){
  workMovementSensor();
  workTouchSensor();
  workBluetooth();
}

void workMovementSensor(){
  long now = millis();
  bool height = digitalRead(moveDetectorPin) == HIGH;
  if (height && !checked) {
    bool timeCheck = now > lastSend + minSecsBetweenSend * countMillisInSec;
    if (timeCheck)
         digitalWrite(ledPinMove, HIGH);
         
         data.setDangerousSituation(countDangerousSituation++);
         outputConsole("Movement: ", data.getDangerousSituation());
         lastSend = now;
        
         delay(oneDelay);
         checked = true;
   }
   else if(!height && checked) {
     digitalWrite(ledPinMove, LOW);
     Serial.println("Too soon");
     delay(oneDelay);
     checked = false;
  }
}

void workTouchSensor(){
  val = digitalRead(touchSensorPin);
  if(val == HIGH) {
     digitalWrite(ledPinTouch, HIGH);
     
     long current = millis();
     long differ = current - oldTimestamp;
     countTime = differ + countTime;
     
     data.setTimeTrip(countTime / countMetrInKm);
     data.setSpeedBike(meedleLenth/(abs((differ/countMillisInSec) - countDelay)));
     data.setDistance((meedleLenth * (countTouch++)) / countMetrInKm);
     data.setCalories(meedleWeight * data.getDistance());
   
     outputConsole("Time seconds: ", data.getTimeTrip());
     outputConsole("Speed m/s ", data.getSpeedBike());
     outputConsole("Distance km: ", data.getDistance());
     outputConsole("Calories count: ", data.getCalories());
        
     delay(oneDelay);
     oldTimestamp = current;
  }
  digitalWrite(ledPinTouch, LOW);
  delay(oneDelay);
}

void workBluetooth(){
  sendDataOut(blueTooth, data); 
}


void outputConsole(String title, float value){
     Serial.print(title);
     Serial.println(value);
}

void sendDataOut(SoftwareSerial blueTooth, StatisticsData data){
  if (Serial.available())  
     {     
       blueTooth.write(data.getTimeTrip()); 
       blueTooth.write(data.getSpeedBike()); 
       blueTooth.write(data.getDistance()); 
       blueTooth.write(data.getCalories()); 
     }
}


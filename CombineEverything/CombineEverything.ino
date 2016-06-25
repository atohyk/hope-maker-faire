//Make your LED vary in brightness and servo move according to the sonar reading
//LED switches off if you press the switch
#include "Servo.h"
Servo myServo;
const int trigPin = 12;
const int echoPin = 11;
const int ledPin = 3;
const int switchPin = 2;
const int servoPin = 5;
unsigned long duration;
const float durationExpSens = 0.1;
const int maxSonar = 2000;
const int minSonar = 200;
int ledValue;
int servoValue;
unsigned char switchValue;
unsigned long serialPrint;
const int servoPrintDelay = 500;
const float servoExpSens = 0.1;
const unsigned char servoMaxAngle = 180;

void setup() {
  // put your setup code here, to run once:
  duration = 0;
  ledValue = 0;
  servoValue = 0;
  switchValue = 0;
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(switchPin, INPUT);
  digitalWrite(trigPin, LOW);
  digitalWrite(switchPin, HIGH);
  myServo.attach(servoPin);
  serialPrint = millis();
}

void loop() {
  // put your main code here, to run repeatedly:

  //Getting Sonar Readings
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = (unsigned long)((1 -durationExpSens) * duration + durationExpSens * pulseIn(echoPin, HIGH));
  

  if(duration > maxSonar){
    duration = maxSonar;
  }
  else if(duration < minSonar){
    duration = minSonar;
  }

  //checking for the switch
  switchValue = digitalRead(switchPin);
  

  //writing the led value
  if(switchValue){
    ledValue = (duration-minSonar)*255/(maxSonar-minSonar);
    analogWrite(ledPin, ledValue);
  }
  else{
    ledValue = 0;
    analogWrite(ledPin, ledValue);
  }
  

  //writing the servo value
  servoValue = (int)((1 - servoExpSens)*servoValue + servoExpSens*(duration-minSonar)*servoMaxAngle/(maxSonar-minSonar));
  myServo.write(servoValue);

  //delaying so this doesn't happen too often and we don't get flooded with information
  if(millis() - serialPrint > servoPrintDelay){
    serialPrint = millis();
    Serial.print("Sonar pulse duration: ");
    Serial.println(duration);
    Serial.print("Switch: ");
    Serial.println(switchValue);
    Serial.print("Led Value: ");
    Serial.println(ledValue);
    Serial.print("Servo Value: ");
    Serial.println(servoValue);
  }
}


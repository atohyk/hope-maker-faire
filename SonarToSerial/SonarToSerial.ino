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
const int maxSonar = 2000;
const int minSonar = 200;
int ledValue;
int servoValue;
unsigned char switchValue;

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
  digitalWrite(switchPin, LOW);
  myServo.attach(servoPin);
}

void loop() {
  // put your main code here, to run repeatedly:

  //Getting Sonar Readings
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  Serial.print("Sonar pulse duration: ");
  Serial.println(duration);

  //checking for the switch
  switchValue = digitalRead(switchPin);
  Serial.print("Switch: ");
  if(switchValue){
    Serial.println("On");
  }
  else{
    Serial.println("Off");
  }

  //writing the led value
  if(!switchValue){
    ledValue = (duration-minSonar)*255/maxSonar;
    analogWrite(ledPin, ledValue);
    Serial.print("Led Value: ");
    Serial.println(ledValue);
  }
  else{
    ledValue = 0;
    analogWrite(ledPin, ledValue);
    Serial.print("Led Value: ");
    Serial.println(ledValue);
  }
  

  //writing the servo value
  servoValue = (duration-minSonar)*180/maxSonar;
  myServo.write(servoValue);
  Serial.print("Servo Value: ");
  Serial.println(servoValue);

  //delaying so this doesn't happen too often and we don't get flooded with information
  delay(500);
  
}


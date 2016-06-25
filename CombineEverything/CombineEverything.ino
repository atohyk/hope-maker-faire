//Make your LED vary in brightness and servo move according to the sonar reading
//LED switches off if you press the switch

//includes
#include "Servo.h"

//instantiating objects
Servo myServo;

//pin configurations
const int trigPin = 12; //trigger pin for the sonar - this pin is to trigger the sonar, pull up 10us
const int echoPin = 11; //echo pin for the sonar - this pin is to read in the pulse from the sonar
const int ledPin = 3; //this is the pin that controls the LED
const int switchPin = 2; //this is the pin that reads in whether the switch is depressed, the switch pulls to gnd
const int servoPin = 5; //this pin is connected to the PWM input of the rc servo

//global variables
unsigned long duration; //this holds the value of the high pulse from the sonar, telling us the distance reading
int ledValue; //this holds the value of the led command, from 0 to 255 for the analogWrite
int servoValue; //this holds the value of the the servo command
unsigned char switchValue; //this stores the value of the switch so that we can print it and use it
unsigned long serialPrint; //this notes down the last time that we've printed to the serial

//constants
const float durationExpSens = 0.1; //strength of the exponential filter for the sonar readings
const int maxSonar = 2000; //max value of the sonar that we'll be considering
const int minSonar = 200; //min value of the sonar that we'll be considering
const int servoPrintDelay = 500; //length of time in milliseconds that we'll wait before the next print to serial
const float servoExpSens = 0.1; //the strength of the exponential filter for the servo commands - smaller is stronger
const unsigned char servoMaxAngle = 180; //the maximum angle of the servo, this is actually decided by the library.

void setup() {
  //initializing all our variables:
  duration = 0;
  ledValue = 0;
  servoValue = 0;
  switchValue = 0;

  //starting the serial channel
  Serial.begin(9600);

  //configuring all the pin modes
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(switchPin, INPUT);

  //important starting pin configurations
  digitalWrite(trigPin, LOW);
  digitalWrite(switchPin, HIGH); //while the switchPin is an input, this line makes it pull high

  //attaching the servos
  myServo.attach(servoPin);

  //note down the first serialprint timer.
  serialPrint = millis();
}

void loop() {
  //Getting Sonar Readings
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  //exponential filtering of the duration
  duration = (unsigned long)((1 -durationExpSens) * duration + durationExpSens * pulseIn(echoPin, HIGH));
  
  //ensuring that the sonar reading falls within the max and min limits
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
    ledValue = (duration-minSonar)*255/(maxSonar-minSonar); //scaling the led value
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


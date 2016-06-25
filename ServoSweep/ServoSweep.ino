//Moves your servo back and forth
#include "Servo.h"
Servo myservo;
const int servoPin = 5;
unsigned char servoUp;
unsigned long servoTime;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  myservo.attach(5);
  servoTime = millis();
  servoUp = 0;
}

void loop() {
  // put your main code here, to run repeatedly:
  if(millis() - servoTime > 1000){
    servoTime = millis();
    if(servoUp){
      myservo.write(180);
      Serial.println("Servo Up");
      servoUp = 0;
    }
    else{
      myservo.write(0);
      Serial.println("Servo Down");
      servoUp = 1;
    }
  }
  
}


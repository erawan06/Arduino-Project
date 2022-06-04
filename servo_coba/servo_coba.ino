#include <Servo.h>       // Include the Servo library 
int servoPin = 9;       

Servo myServo;           // Create a servo object
int val = 0;
long duration;
int distance;

void setup() {
  myServo.attach(servoPin);
  Serial.begin(9600);       //start serial
}

void loop() {

 
  if (Serial.available()) {           //Check data

    int val = Serial.read();
    myServo.write(val);
    delay(15);


  }
}

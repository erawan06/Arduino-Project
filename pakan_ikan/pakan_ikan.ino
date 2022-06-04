#include <Servo.h>             //Servo library
 
Servo servo_test;        //initialize a servo object for the connected servo  
 
void setup(){ 
  Serial.begin(9600);
  servo_test.attach(9);      // attach the signal pin of servo to pin9 of arduino
  servo_test.write(180);
} 
  
void loop(){  
  if(millis() == 86400000){
    servo_test.write(90);                 //command to rotate the servo to the specified angle 
    delay(5000);
    servo_test.write(180);              //command to rotate the servo to the specified angle
    delay(5000);
  }                              
}

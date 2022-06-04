//-------------------------------------------------------------------------------------
// HX711_ADC.h
// Arduino master library for HX711 24-Bit Analog-to-Digital Converter for Weigh Scales
// Olav Kallhovd sept2017
// Tested with      : HX711 asian module on channel A and YZC-133 3kg load cell
// Tested with MCU  : Arduino Nano, ESP8266
//-------------------------------------------------------------------------------------
// This is an example sketch on how to use this library
// Settling time (number of samples) and data filtering can be adjusted in the config.h file

#include <HX711_ADC.h>
#include <EEPROM.h>

//HX711 constructor (dout pin, sck pin):
HX711_ADC LoadCell(4, 5);

const int eepromAdress = 0;
static int flag_naik,flag_turun;
long t;

const int pwm = D4 ;  //enable motor output 1
const int in_1 = D5 ; //
const int in_2 = D8 ;//

const int pwm2 = D3 ;  //enable motor output 2
const int in_11 = D0 ; //
const int in_22 = D7 ;//

void setup() {
  pinMode(pwm, OUTPUT) ;  //we have to set PWM pin as output
  pinMode(in_1, OUTPUT) ; //Logic pins are also set as output
  pinMode(in_2, OUTPUT) ;
  pinMode(pwm2, OUTPUT) ;  //we have to set PWM pin as output
  pinMode(in_11, OUTPUT) ; //Logic pins are also set as output
  pinMode(in_22, OUTPUT) ;
  float calValue; // calibration value
  //  calValue = 696.0; // uncomment this if you want to set this value in the sketch
#if defined(ESP8266)
  EEPROM.begin(512); // uncomment this if you use ESP8266 and want to fetch the value from eeprom
#endif
  EEPROM.get(eepromAdress, calValue); // uncomment this if you want to fetch the value from eeprom

  Serial.begin(9600); delay(10);
  Serial.println();
  Serial.println("Starting...");
  LoadCell.begin();
  long stabilisingtime = 2000; // tare preciscion can be improved by adding a few seconds of stabilising time
  LoadCell.start(stabilisingtime);
  if (LoadCell.getTareTimeoutFlag()) {
    Serial.println("Tare timeout, check MCU>HX711 wiring and pin designations");
  }
  else {
    LoadCell.setCalFactor(calValue); // set calibration value (float)
    Serial.println("Startup + tare is complete");
  }
}

void loop() {
  //update() should be called at least as often as HX711 sample rate; >10Hz@10SPS, >80Hz@80SPS
  //use of delay in sketch will reduce effective sample rate (be carefull with use of delay() in the loop)
  LoadCell.update();

  //get smoothed value from data set
  if (millis() > t + 250) {
    float i = LoadCell.getData();
    Serial.print("Load_cell output val: ");
    Serial.println(i);
    t = millis();

    if (i > 100) {
      Serial.println("naik");
      if (flag_naik == 1) {
        //For brake
        digitalWrite(in_1, HIGH) ;
        digitalWrite(in_2, HIGH) ;
        digitalWrite(in_11, HIGH) ;
        digitalWrite(in_22, HIGH) ;
        delay(1000) ;
        Serial.println("Sek Hujan");
      }
      else {
        digitalWrite(in_1, HIGH) ; // bismillah iki paten
        digitalWrite(in_2, LOW) ;
        digitalWrite(in_11, HIGH) ;
        digitalWrite(in_22, LOW) ;
        analogWrite(pwm, 10000) ;
        analogWrite(pwm2, 10000) ;
        delay(30);
      }
      flag_naik = 1;
      flag_turun = 0;
    }
    else {
      Serial.println("turun");
      if (flag_turun == 1) {
        //For brake
        digitalWrite(in_1, HIGH) ;
        digitalWrite(in_2, HIGH) ;
        digitalWrite(in_11, HIGH) ;
        digitalWrite(in_22, HIGH) ;
        delay(1000) ;
        Serial.println("Brake");
      }
      else {
        digitalWrite(in_1, LOW) ;
        digitalWrite(in_2, HIGH) ;
        digitalWrite(in_11, LOW) ;
        digitalWrite(in_22, HIGH) ;
        analogWrite(pwm, 10000) ;
        analogWrite(pwm2, 10000) ;
        delay(30);
        Serial.println("aku turun");
      }
      flag_naik = 0;
      flag_turun = 1;
    }
  }

  //receive from serial terminal
  if (Serial.available() > 0) {
    float i;
    char inByte = Serial.read();
    if (inByte == 't') LoadCell.tareNoDelay();
  }

  //check if last tare operation is complete
  if (LoadCell.getTareStatus() == true) {
    Serial.println("Tare complete");
  }

}

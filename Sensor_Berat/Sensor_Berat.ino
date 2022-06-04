#include "HX711.h"
#define DOUT  D4 //DT
#define CLK  D3 // SCK
HX711 scale(DOUT, CLK);
float calibration_factor = -50;
int GRAM,manipulasi;

void setup() {
  Serial.begin(115200);
  Serial.println("tekan a,s,d,f untuk menaikan calibration_factor ke 10,100,1000,10000");
  Serial.println("tekan z,x,c,v untuk menurunkan calibration_factor ke 10,100,1000,10000");
  Serial.println("Tekan T untuk Tare");
  scale.set_scale();
  scale.tare();
}

void loop() {
  scale.set_scale(calibration_factor);
  GRAM = scale.get_units(), 10;
  if(GRAM < 0){
    GRAM = 0;
  }
  manipulasi = GRAM + 100;
  Serial.print("Reading: ");
  Serial.print(GRAM);
  Serial.print(" Gram");
  Serial.print(" calibration_factor: ");
  Serial.print(calibration_factor);
  Serial.println();
  delay(1000);
  

  if (Serial.available()) {
    char temp = Serial.read();
    if (temp == '+' || temp == 'a')
      calibration_factor += 0.1;
    else if (temp == '-' || temp == 'z')
      calibration_factor -= 0.1;
    else if (temp == 's')
      calibration_factor += 10;
    else if (temp == 'x')
      calibration_factor -= 10;
    else if (temp == 'd')
      calibration_factor += 100;
    else if (temp == 'c')
      calibration_factor -= 100;
    else if (temp == 'f')
      calibration_factor += 1000;
    else if (temp == 'v')
      calibration_factor -= 1000;
    else if (temp == 't')
      scale.tare();
  }
}

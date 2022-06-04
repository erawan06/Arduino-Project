#include <SoftwareSerial.h>
#define rxPin 8
#define txPin 7

SoftwareSerial gySerial(rxPin, txPin);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);   // serial monitor
  gySerial.begin(115200); // serial gy25
  delay(3000); // Jeda 3 detik

  // Kalibrasi Tilt
  gySerial.write(0xA5);
  gySerial.write(0x54);
  delay(1000); // jeda sebelum kalibrasi heading

  // kalibrasi heading
  gySerial.write(0xA5);
  gySerial.write(0x55);

  delay(100); // Jeda sebelum konfigurasi output

  // Output ASCII
  gySerial.write(0xA5);
  gySerial.write(0x53);

  delay(100);
}

byte rx_byte = 0;

void loop() {
  // put your main code here, to run repeatedly:
  /*
  if(Serial.available()) {
    rx_byte = Serial.read();
    gySerial.write(rx_byte);    
  }

    
  if(gySerial.available()) {
    rx_byte = gySerial.read();
    Serial.write(rx_byte);  
  }
  */

  // baca data gy25 secara realtime
  Serial.println(gySerial.readStringUntil('\n'));
}

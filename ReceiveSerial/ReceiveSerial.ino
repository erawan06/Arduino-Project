unsigned char START_BYTE = 0x53;
unsigned char counterValue = 0;
unsigned char staticValue = 5;
unsigned char checksum = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  unsigned char rxByte = 0;
  unsigned char calculatedChecksum = 0;
  unsigned char syncByteFound = 0;
  
  if(Serial2.available() > 0) {
    if(syncByteFound == 0) {
      rxByte = Serial2.read();
      if(rxByte == 0x53) syncByteFound = 1;
    }
  }

  if(Serial2.available() > 2) {
    counterValue = Serial2.read();
    staticValue = Serial2.read();
    checksum = Serial2.read();

    calculatedChecksum = counterValue + staticValue;

    Serial.print("["); Serial.print("S"); Serial.print("]");
    Serial.print("["); Serial.print(counterValue); Serial.print("]");
    Serial.print("["); Serial.print(staticValue); Serial.print("]");
    Serial.print("["); Serial.print(checksum); Serial.print("]");

    if(calculatedChecksum == checksum) Serial.println("[Checksum Passed]");
    else Serial.println("[Checksum Failed]");
    syncByteFound = 0;
  }

}

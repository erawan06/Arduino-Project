int rainsense= D8; // analog sensor pin A0 untuk input raindrop

void setup() {
  Serial.begin(115200);
}

void loop() {
  int rainSenseReading = analogRead(rainsense);
  //Serial.println(rainSenseReading); // untuk monitoring output bila hujan
  delay(250);// hujan value dari 0 ke 1023.
  if(rainSenseReading <500){
    Serial.println("Hujan");
    }
  else{ // jika tidak hujan
    Serial.println("Tidak Hujan");
    }
   delay(1000);
}

#include "SPI.h" // SPI library
#include "MFRC522.h" // RFID library (https://github.com/miguelbalboa/rfid)
//#include "Servo.h"

#define SS_PIN    8
#define RST_PIN   9
//#define hijau     5
//#define merah     6
//#define BuzzerPin 4
//#define servo     3

//Deklarasi gak ero opo iki
MFRC522 mfrc522(SS_PIN, RST_PIN); // Set up mfrc522 on the Arduino
//Servo myservo;


void setup() {
  SPI.begin(); // open SPI connection
  mfrc522.PCD_Init(); // Initialize Proximity Coupling Device (PCD)
  Serial.begin(9600); // open serial connection
  //pinMode(hijau,OUTPUT);
  //pinMode(merah,OUTPUT);
  //pinMode(BuzzerPin,OUTPUT);
  //myservo.attach(servo);
  //myservo.write(0);
  Serial.println("Ready cuy");
}

void RFID(){
  if (mfrc522.PICC_IsNewCardPresent()) { // (true, if RFID tag/card is present ) PICC = Proximity Integrated Circuit Card
    if(mfrc522.PICC_ReadCardSerial()) { // true, if RFID tag/card was read
      //Serial.print("RFID TAG ID:");
      String data;
      //Serial.print(mfrc522.uid.uidByte[1]);
      for (byte i = 0; i < mfrc522.uid.size; ++i) { // read id (in parts)
        //Serial.print(mfrc522.uid.uidByte[i], HEX); // print id as hex values
        //Serial.print(" "); // add space between hex blocks to increase readability
        data.concat(String(mfrc522.uid.uidByte[i], HEX));
        }
      
      //Serial.println("");
      data.toUpperCase();
      Serial.print(data);
//      Serial.println('_'); // Print out of id is complete.
//      //delay(4000);
//      myservo.write(90);
//      digitalWrite(BuzzerPin, HIGH);
//      delay(100);
//      digitalWrite(BuzzerPin, LOW);
//      digitalWrite(hijau,HIGH);
//      delay(5000); 
//      digitalWrite(BuzzerPin, HIGH);
//      delay(100);
//      digitalWrite(BuzzerPin, LOW);
//      myservo.write(0);  
//      digitalWrite(hijau,LOW);
      if (data == "1E75D96E") 
      {
        Serial.println("Yoi");
    //Serial.println("AKSES DIIZINKAN");
    //myservo.write(90);
    //digitalWrite(BuzzerPin, HIGH);
    //delay(100);
    //digitalWrite(BuzzerPin, LOW);
    //digitalWrite(hijau,HIGH);
    //delay(5000); 
    //digitalWrite(BuzzerPin, HIGH);
    //delay(100);
    //digitalWrite(BuzzerPin, LOW);
    //myservo.write(0);  
    //digitalWrite(hijau,LOW);
    }
    
    else   {
      Serial.println(" Coba lagi bos");
    //Serial.println("AKSES DITOLAK!");
    //digitalWrite(BuzzerPin, HIGH);
    //digitalWrite(merah,HIGH);
    //delay(500);
    //digitalWrite(merah,LOW);
    //digitalWrite(BuzzerPin, LOW);
    }
    }
    }
}

void loop() {
  // put your main code here, to run repeatedly:
  RFID();

}

//#include <SPI.h>
//#include <MFRC522.h>
 
//#define SS_PIN 10
//#define RST_PIN 9
//#define LED_G 5 //define green LED pin
//#define LED_R 4 //define red LED
#define RELAY 8 //relay pin
//#define BUZZER 2 //buzzer pin
#define DENIED_DELAY 1000
//MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
 
void setup() 
{
  Serial.begin(9600);   // Initiate a serial communication
  //SPI.begin();          // Initiate  SPI bus
  //pinMode(LED_G, OUTPUT);  mfrc522.PCD_Init();   // Initiate MFRC522

  //pinMode(LED_R, OUTPUT);
  pinMode(RELAY, OUTPUT);
  //pinMode(13, OUTPUT);
  //pinMode(BUZZER, OUTPUT);
  //noTone(BUZZER);
  digitalWrite(RELAY, HIGH);
  //Serial.printl ACCESS_DELAY 2000
  //definen("Put your card to the reader...");
  //Serial.println();

}

void loop() {
  digitalWrite(RELAY, LOW);
  delay(3000);
  digitalWrite(RELAY, HIGH);
  delay(3000);
}

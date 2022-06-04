#include "CTBot.h"
CTBot myBot;
String ssid = "xxxxx";
String pass = "xxxxx";
String token = "xxxxx";
//const int id = xxxxx;
#define Relay  D0

void setup() {
  Serial.begin(115200);
  Serial.println("Starting TelegramBot...");
  myBot.wifiConnect(ssid, pass);
  myBot.setTelegramToken(token);
  pinMode(Relay,OUTPUT);

  if (myBot.testConnection()) {
    Serial.println("Koneksi Bagus");
  } else {
    Serial.println("Koneksi Jelek");
  }
  
 // myBot.sendMessage(id, "OK bisa");
 // Serial.println("Pesan Terkirim");
 
}

void loop() {
  TBMessage msg;
  
 if (myBot.getNewMessage(msg)) {
  if(msg.text == "ok"){
  digitalWrite(Relay, LOW); //kunci terbuka
  delay(1000);  //delay kunci terbuka selama 5000ms atau 5s
  }
  if(msg.text == "ko"){
  digitalWrite(Relay, HIGH); //kunci tertutup
  delay(1000);
  }
 }
}

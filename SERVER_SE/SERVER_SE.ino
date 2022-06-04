/*
  NODE MASTER
  IP ADDRESS : 192.168.43.85
  PORT : 4210
*/
  
#include <ESP8266WiFi.h>
#include <WiFiUDP.h>

#define portNodeClient1 4210

char ip_addreNodeClient1[] = ("192.168.137.71");

const int pwm = D4 ;  //enable motor output 1
const int in_1 = D5 ; // 
const int in_2 = D8 ;// 

const int pwm2 = D3 ;  //enable motor output 2
const int in_11 = D1 ; //
const int in_22 = D2 ;//

static int flag_naik,flag_turun;
int rainsense = D6; // analog sensor pin A0 untuk input raindrop

const int nodeclient1 = 11;

WiFiUDP Udp;
//
//IPAddress staticIP(192, 168, 43, 85);
//IPAddress gateway(192, 168, 43, 1);
//IPAddress subnet(255, 255, 255, 0);
//IPAddress dns(192, 168, 43, 1);

unsigned int localUdpPort = 4210;
//char incomingPacket[256];
char replyPacket[] = ("HALO CLIENT INI SERVER");

void sendingData(int client) {
  Udp.beginPacket(ip_addreNodeClient1, portNodeClient1);
  Udp.write(replyPacket);
  Udp.endPacket();

}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println();
  WiFi.disconnect();
  
  pinMode(pwm,OUTPUT) ;   //we have to set PWM pin as output
  pinMode(in_1,OUTPUT) ;  //Logic pins are also set as output
  pinMode(in_2,OUTPUT) ;
  pinMode(pwm2,OUTPUT) ;   //we have to set PWM pin as output
  pinMode(in_11,OUTPUT) ;  //Logic pins are also set as output
  pinMode(in_22,OUTPUT) ;
  Serial.printf("Connecting to ...");

  WiFi.begin("VIVAT ITS", "cumacobacoba");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  delay(1000);
  Udp.begin(localUdpPort);
}

void loop() {
  int packetSize = Udp.parsePacket();
  int rainSenseReading = analogRead(rainsense);
  delay(250);// hujan value dari 0 ke 1023.
  if (rainSenseReading < 500) {
    sendingData(nodeclient1);
    Serial.println("Hujan");
    if(flag_naik == 1){
      sendingData(nodeclient1);
      //For brake
      digitalWrite(in_1,HIGH) ;
      digitalWrite(in_2,HIGH) ;
      digitalWrite(in_11,HIGH) ;
      digitalWrite(in_22,HIGH) ;
      delay(1000) ;
      Serial.println("Sek Hujan");
      }
    else { 
      sendingData(nodeclient1);
      digitalWrite(in_1,LOW) ;
      digitalWrite(in_2,HIGH) ;
      digitalWrite(in_11,LOW) ;
      digitalWrite(in_22,HIGH) ;
      analogWrite(pwm,2000) ;
      analogWrite(pwm2,2000) ;
      delay(15);
      } 
    sendingData(nodeclient1);
    flag_naik = 1;  
    flag_turun = 0;
   }
 else {
  Serial.println("tidak hujan");
  if(flag_turun == 1){
      //For brake
      digitalWrite(in_1,HIGH) ;
      digitalWrite(in_2,HIGH) ;
      digitalWrite(in_11,HIGH) ;
      digitalWrite(in_22,HIGH) ;
      delay(1000) ;
      Serial.println("Brake");
      }
    else { 
      digitalWrite(in_1,HIGH) ;
      digitalWrite(in_2,LOW) ;
      digitalWrite(in_11,HIGH) ;
      digitalWrite(in_22,LOW) ;
      analogWrite(pwm,2500) ;
      analogWrite(pwm2,2500) ;
      delay(15);
      }
  flag_naik = 0;
  flag_turun = 1;
 }
}

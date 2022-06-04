/*
*  NODE CLIENT1
*  IP ADDRESS : 192.168.43.139
*  PORT       : 4211
*/

#include <ESP8266WiFi.h>
#include <WiFiUDP.h>

#define portNodeMaster  4210

WiFiUDP Udp;

//IPAddress staticIP(192,168,43,139);
//IPAddress gateway(192,168,43,1);
//IPAddress subnet(255,255,255,0);
//IPAddress dns(192,168,43,1);

unsigned int localUdpPort = 4210;
char incomingPacket[256];

const int pwm = D4 ;  //enable motor output 1
const int in_1 = D5 ; // 
const int in_2 = D8 ;// 

const int pwm2 = D3 ;  //enable motor output 2
const int in_11 = D1 ; //
const int in_22 = D2 ;//

static int flag_naik,flag_turun;

void receivingDta(int packtsize)
{
    // receive incoming UDP packets
    //Serial.printf("Received %d bytes from %s, port %d\n", packtsize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
    int len = Udp.read(incomingPacket, 255);
    if (len > 0)
    {
      incomingPacket[len] = 0;
    }
     if(flag_naik == 1){
      //For brake
      digitalWrite(in_1,HIGH) ;
      digitalWrite(in_2,HIGH) ;
      digitalWrite(in_11,HIGH) ;
      digitalWrite(in_22,HIGH) ;
      delay(1000) ;
      Serial.println("Sek Hujan");
      }
    else {
      digitalWrite(in_1,LOW) ;
      digitalWrite(in_2,HIGH) ;
      digitalWrite(in_11,LOW) ;
      digitalWrite(in_22,HIGH) ;
      analogWrite(pwm,2500) ;
      analogWrite(pwm2,2500) ;
      delay(15);
      }
      flag_naik = 1;  
      flag_turun = 0;
    Serial.printf("%s\n", incomingPacket);
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

  //WiFi.config(staticIP, subnet, gateway, dns);
  WiFi.begin("VIVAT ITS", "cumacobacoba");
  WiFi.mode(WIFI_STA);
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
  if(packetSize){
    receivingDta(packetSize);
    Serial.println("Hujan");
    if(flag_naik == 1){
      //For brake
      //receivingDta(packetSize);
      digitalWrite(in_1,HIGH) ;
      digitalWrite(in_2,HIGH) ;
      digitalWrite(in_11,HIGH) ;
      digitalWrite(in_22,HIGH) ;
      delay(1000) ;
      Serial.println("Sek Hujan");
      }
    else {
      //receivingDta(packetSize);
      digitalWrite(in_1,LOW) ;
      digitalWrite(in_2,HIGH) ;
      digitalWrite(in_11,LOW) ;
      digitalWrite(in_22,HIGH) ;
      analogWrite(pwm,2500) ;
      analogWrite(pwm2,2500) ;
      delay(15);
      }
      //receivingDta(packetSize);
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

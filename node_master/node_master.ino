/*
*  NODE MASTER
*  IP ADDRESS : 192.168.43.85
*  PORT       : 4210
*/

#include <ESP8266WiFi.h>
#include <WiFiUDP.h>

#define portNodeClient1   4210

char ip_addreNodeClient1[] = ("192.168.137.96");
int rainsense= D6; // analog sensor pin A0 untuk input raindrop

const int nodeclient1 = 11;

WiFiUDP Udp;

IPAddress staticIP(192,168,1,111);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);
IPAddress dns(192,168,1,1);

unsigned int localUdpPort = 4210;
//char incomingPacket[256];
char replyPacket[] = ("HALO CLIENT INI SERVER");

void sendingData(int client){
        Udp.beginPacket(ip_addreNodeClient1, portNodeClient1);
        Udp.write(replyPacket);
        Udp.endPacket();      
      
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println();
  WiFi.disconnect();
  pinMode(BUILTIN_LED, OUTPUT);// engkuk disetting nang ken motor e
  
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
  if(rainSenseReading <500){
    sendingData(nodeclient1);
    Serial.println("Hujan");
    digitalWrite(BUILTIN_LED, HIGH);//engkuk kodingan motor deleh kene
    delay(5000);
    digitalWrite(BUILTIN_LED, LOW);
    delay(5000);
    }
  else{ // jika tidak hujan
    Serial.println("Tidak Hujan");
    }
  
  delay(1000);  
}

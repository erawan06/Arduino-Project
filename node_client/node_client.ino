/*
*  NODE CLIENT1
*  IP ADDRESS : 192.168.43.139
*  PORT       : 4211
*/

#include <ESP8266WiFi.h>
#include <WiFiUDP.h>

#define portNodeMaster  4210

WiFiUDP Udp;

IPAddress staticIP(192,168,43,139);
IPAddress gateway(192,168,43,1);
IPAddress subnet(255,255,255,0);
IPAddress dns(192,168,43,1);

unsigned int localUdpPort = 4210;
char incomingPacket[256];

void receivingDta(int packtsize)
{
    // receive incoming UDP packets
    //Serial.printf("Received %d bytes from %s, port %d\n", packtsize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
    int len = Udp.read(incomingPacket, 255);
    if (len > 0)
    {
      incomingPacket[len] = 0;
    }
    Serial.printf("%s\n", incomingPacket);
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println();
  WiFi.disconnect();
  pinMode(BUILTIN_LED, OUTPUT);// engkuk setting nang kene
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

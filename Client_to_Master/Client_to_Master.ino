/*
*  NODE CLIENT1
*  IP ADDRESS : 192.168.43.139
*  PORT       : 4211
*/

#include <ESP8266WiFi.h>
#include <WiFiUDP.h>

#define portNodeMaster  4210


WiFiUDP Udp;

IPAddress staticIP(192,168,1,142);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);
IPAddress dns(192,168,1,1);

//IPAddress staticIP(192,168,100,59);
//IPAddress gateway(192,168,100,1);
//IPAddress subnet(255,255,255,0);
//IPAddress dns(192,168,100,1);

unsigned int localUdpPort = 4214;
char incomingPacket[256];
char replyPacket[] = ("00");
char lastPacket[] = ("00");
//int replyPacket = 1;

const int  buttonPin = D1;
const int ledPin1 =  D7;
const int ledPin2 =  D8;
int buttonState = 0,blinkState = 0;
int nilaibutton,sudah_print_on=true,sudah_print_off=true;
int data=0;

void receivingDta(int packtsize)
{
    // receive incoming UDP packets
    Serial.printf("Received %d bytes from %s, port %d\n", packtsize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
    int len = Udp.read(incomingPacket, 255);
    if (len > 0)
    {
      incomingPacket[len] = 0;
    }
    
    pilih();
    Serial.printf("UDP packet contents: %s\n", incomingPacket);
  
}

void sendingDta()
{
    // send back a reply, to the IP address and port we got the packet from
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.write(replyPacket);
    Udp.endPacket();
    Serial.println("Terkirim!");

//    // send back a reply, to the IP address and port we got the packet from
//    Udp.beginPacket(ip_addreNodeMaster, portNodeMaster);
//    Udp.write(replyPacket);
//    Udp.endPacket();
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println();
  WiFi.disconnect();
  Serial.printf("Connecting to ...");

  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(buttonPin, INPUT);

  WiFi.config(staticIP, subnet, gateway, dns);
  //WiFi.begin("FAMILY", "mamasulami");
  //WiFi.begin("Around", "keliling");
  WiFi.begin("nodemcu", "nodemcuwifi");
  WiFi.mode(WIFI_STA);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" connected");

  Udp.begin(localUdpPort);
  Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), localUdpPort);
}

void loop() {
  button();
  /* ------------ receiving data from master ------------ */
  int packetSize = Udp.parsePacket();
  if(packetSize)  receivingDta(packetSize);
//  else if(strcmp(lastPacket,replyPacket)!=0){
//    //Serial.println("No incoming data");
//    sendingDta();
//    lastPacket[0] = replyPacket[0];
//    lastPacket[1] = replyPacket[1];
//  }
  
  /* ------------ sending data to master ---------------- */
  
//  if(Serial.available()>0){
//    data = Serial.read();
//    pilih();
//  }
  delay(10);
}

void button(){
  //Serial.print("123\n");
  buttonState = digitalRead(buttonPin);
  if(buttonState == LOW){
    if(sudah_print_on){
      sudah_print_on=false;
      sudah_print_off=true;
      //Serial.write("11");
      replyPacket[0] = '4';
      replyPacket[1] = '1';
      //Serial.println("sending 11");
      sendingDta();
      digitalWrite(ledPin1, HIGH);
      digitalWrite(ledPin2, HIGH);
    }
  }else {
   if(sudah_print_off){
     sudah_print_on=true;
     sudah_print_off=false;
     //Serial.write("10");
     replyPacket[0] = '4';
     replyPacket[1] = '0';
     //Serial.println("sending 10");
     sendingDta();
     digitalWrite(ledPin1, LOW);
     digitalWrite(ledPin2, LOW);
    }
  }
  //delay(100);
}

void pilih(){
  data = atoi(incomingPacket);
  switch (data){
    case 41 : digitalWrite(ledPin2, HIGH); break;
    case 40 : digitalWrite(ledPin2, LOW); break;
    default: break;
  }
}

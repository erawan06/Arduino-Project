#include <SPI.h> // library to connect arduino and ehternet shield
#include <Ethernet.h> //Ethernet shield library
#include "MFRC522.h" // RFID library (https://github.com/miguelbalboa/rfid)

/////////Declaration pin////////
#define SS_PIN    8
#define RST_PIN   9
#define RELAY     2
#define BuzzerPin A2
#define merah     A1
#define hijau     A0
#define button    A3
////////-----------///////////

///////Deklarasi Variable///////
MFRC522 mfrc522(SS_PIN, RST_PIN); // Set up mfrc522 on the Arduino
byte mac[] = { 0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02 }; //physical mac address
byte ip[] = {192,168,137,36}; //alamat IP ethernetshield
byte serve[] = {192,168,137,1}; //alamat IP server,cara lihat gunakan perintah ipcofig
String value,rcv; //variable value digunakan untuk mengirim ID card ke server
                 //variable rcv digunakan untuk menampung pembacaan data dari server
int buttonState; //sebagai flag button
EthernetClient cliente;
//////-------------///////

void setup() {
  SPI.begin(); // open SPI connection
  mfrc522.PCD_Init(); // Initialize Proximity Coupling Device (PCD)
  pinMode(RELAY, OUTPUT); //Membuat pin 2 sebagai output relay
  digitalWrite(RELAY, HIGH); //Membuat kondisi default relay menjadi HIGH
  pinMode(hijau,OUTPUT); //Membuat pin A0 sebagai output LED hijau
  pinMode(merah,OUTPUT); //Membuat pin A1 sebagai output LED merah
  pinMode(BuzzerPin,OUTPUT); //Membuat pin A2 sebagai output buzzer
  pinMode(button, INPUT); //Membuat pin A3 sebagai input dari button
  Serial.begin(9600); //setting the baud rate at 9600
  Ethernet.begin(mac, ip); //open ethernet shield connection
}

void BUTTON(){
  buttonState = digitalRead(button); //membaca input dari button
  //jika button ditekan maka kondisi akan menjadi high
  if (buttonState == HIGH) {
    digitalWrite(merah,LOW); //LED merah mati
    digitalWrite(hijau,HIGH); //LED hijau nyala
    digitalWrite(BuzzerPin, HIGH); //Buzzer berbunyi
    delay(100); //delay buzzer berbunyi selama 100ms
    digitalWrite(BuzzerPin, LOW); //Buzzer mati
    digitalWrite(RELAY, LOW); //kunci terbuka
    delay(5000);  //delay kunci terbuka selama 5000ms atau 5s
    digitalWrite(BuzzerPin, HIGH); //Buzzer berbunyi
    delay(100); //delay buzzer berbunyi selama 100ms
    digitalWrite(BuzzerPin, LOW); //Buzzer mati
    digitalWrite(hijau,LOW); //LED hijau mati
    digitalWrite(RELAY, HIGH); //kunci tertutup
    }
  //////----------------//////////  
}

void RFID(){
  digitalWrite(merah,HIGH); //LED merah nyala
  if (mfrc522.PICC_IsNewCardPresent()) { // (true, if RFID tag/card is present ) PICC = Proximity Integrated Circuit Card
    if(mfrc522.PICC_ReadCardSerial()) { // true, if RFID tag/card was read
      digitalWrite(hijau,HIGH); //LED hijau nyala
      String data; //untuk menampung pembacaan karakter ID card
      for (byte i = 0; i < mfrc522.uid.size; ++i) { // read id (in parts)
        data.concat(String(mfrc522.uid.uidByte[i], HEX)); //menjadikan karakter dalam string data
        }
      data.toUpperCase(); //membuat karakter menjadi huruf kapital
      value = data; //mengisi variable value dengan data
      if (cliente.connect(serve,80)) { //Connecting at the IP address and port we saved before
        Serial.println("connected"); //untuk debug apakah sudah terhubung dengan server
        /////Connecting and Sending values to database//////
        //cliente.print("GET /projectkpagain/write_data.php?"); //Connecting and Sending values to database
        cliente.print("GET /datakaryawan/write_data.php?"); //Connecting and Sending values to database
        cliente.print("value=");
        cliente.print(value);
        cliente.println(" HTTP/1.1");
        cliente.println("Host: 192.168.137.1");
        cliente.println();
       ///////------------////////////
        
        /////check data dari database///////////
        //cliente.print("GET /projectkp/data.php?"); //Connecting and Sending values to database
        cliente.print("GET /datakaryawan/data_data.php?");
        cliente.print("value=");
        cliente.print(value);
        cliente.println(" HTTP/1.1");
        cliente.println("Host: 192.168.137.1");
        cliente.println();
        while(cliente.connected()){
          while(cliente.available()){
            rcv = cliente.readStringUntil('\n');
            }
        }
        /////------------/////////////
        
        cliente.stop(); //Closing the connection
        
        Serial.print("card="); //untuk debug
        Serial.println(value); //untuk debug
        }
      else {// if you didn't get a connection to the server:
        Serial.println("connection failed");
        }
      digitalWrite(hijau,LOW); //LED hijau mati

      //Mengecek hasil pembacaan dari database///////////// 
      if (rcv == "Ready") {
        digitalWrite(merah,LOW); //LED merah mati
        digitalWrite(hijau,HIGH); //LED hijau nyala
        digitalWrite(BuzzerPin, HIGH); //Buzzer berbunyi
        delay(100); //delay buzzer berbunyi selama 100ms
        digitalWrite(BuzzerPin, LOW); //Buzzer mati
        digitalWrite(RELAY, LOW); //kunci terbuka
        delay(5000);  //delay kunci terbuka selama 5000ms atau 5s
        digitalWrite(BuzzerPin, HIGH); //Buzzer berbunyi
        delay(100); //delay buzzer berbunyi selama 100ms
        digitalWrite(BuzzerPin, LOW); //Buzzer mati
        digitalWrite(hijau,LOW); //LED hijau mati
        digitalWrite(RELAY, HIGH); //kunci tertutup
        }
      else {
        digitalWrite(merah,HIGH); //LED merah menyala
        digitalWrite(BuzzerPin, HIGH); //Buzzer berbunyi
        delay(500); //delay buzzer berbunyi selama 100ms
        digitalWrite(merah,LOW); //LED merah mati
        digitalWrite(BuzzerPin, LOW); //Buzzer mati
        }
      ///////------------////////////////  
        }
        }
}
////main program yang di loop//////
void loop() {
   RFID(); //menjalankan fungsi RFID
   BUTTON(); //menjalankan fungsi BUTTON
}
////-----------//////////////////

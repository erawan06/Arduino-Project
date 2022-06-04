// Pendeteksi Hujan

int rainsense= A0; // analog sensor pin A0 untuk input raindrop
int buzzerout= 7; // digital pin nomor 7 untuk output buzzer
int countval= 0; // counter value mulai 0 
int ledout= 13; // digital pin nomor 13 untuk output led
 
void setup(){
   Serial.begin(9600);
   pinMode(buzzerout, OUTPUT);
   pinMode(ledout, OUTPUT); 
   pinMode(rainsense, INPUT);
}
void loop(){
   int rainSenseReading = analogRead(rainsense);
   Serial.println(rainSenseReading); // untuk monitoring output bila hujan
   delay(250);// hujan value dari 0 ke 1023.
   // dari hujan sampai tidak hujan
   if (countval >= 1){                // delay percount
      Serial.print("Hujan");
      digitalWrite(buzzerout, HIGH);  // buzzer nyala
      digitalWrite(ledout, HIGH);     // led nyala
   }
   if (rainSenseReading <500){ 
      countval++; // naik count value
   }
   else if (rainSenseReading >500) { // jika tidak hujan
      digitalWrite(buzzerout, LOW); // buzzer mati
      digitalWrite(ledout, LOW); // led mati
      countval = 0; // reset count ke 0
   }
   delay(1000);
}


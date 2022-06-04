#define Led D1
#define Pot A0

void setup() {
  Serial.begin(115200);
  pinMode(Led, OUTPUT);
  pinMode(Pot, INPUT);
}
 
void loop() {
  int nilaiPot = analogRead(Pot);
  Serial.println(nilaiPot);
  if(nilaiPot > 100) {
    digitalWrite(Led, HIGH);
    }
  else {                    
    digitalWrite(Led, LOW);
  }
  
}

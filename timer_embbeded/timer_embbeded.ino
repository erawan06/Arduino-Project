#define button A3
unsigned int tick,milsec=0,sec=0,menit=0,jam=0,buttonstate;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  TCCR1A = 0x00;
  TCCR1B = 0x04;
  TCNT1H = 0x00;
  TCNT1L = 0x00;
  pinMode (button,INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
  buttonstate = digitalRead(button);
  if (buttonstate == LOW){
    tick = TCNT1;
    if(tick % 625){
      TCNT1 = 0;
      milsec++;
      if(milsec >= 100){
        milsec = 0;
        sec ++;
        if(sec >= 60){
          sec = 0;
          menit++;
          if(menit >= 60){
            menit = 0;
            jam++;
            }
          }
      }
     Serial.print(jam);
     Serial.print(":");
     Serial.print(menit);
     Serial.print(":");
     Serial.print(sec);
     Serial.print(":");
     Serial.println(milsec);
  }
 }
}

#define button 2
unsigned long counter = 0;
unsigned long millisec = 0;
unsigned long microsec = 0;
unsigned long m = 0;
 
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  TCCR1A=(0<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<WGM11) | (0<<WGM10);
  TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (1<<WGM12) | (0<<CS12) | (0<<CS11) | (1<<CS10);
  TCNT1H=0x00;
  TCNT1L=0x00;
  ICR1H=0x00;
  ICR1L=0x00;
  OCR1AH=0x00;
  OCR1AL=0x9F;
  OCR1BH=0x00;
  OCR1BL=0x00;
  //enable interrupt
  TIMSK1=(0<<ICIE1) | (0<<OCIE1B) | (1<<OCIE1A) | (0<<TOIE1);
  pinMode(button,INPUT);
  Serial.println("Ready");
}

void loop() {
  // put your main code here, to run repeatedly:
  if (digitalRead(button) == 1){
    Serial.print("Millis  :");
    Serial.println(millisec);
    Serial.print("Microsec :");
    Serial.println(microsec);
    Serial.println("");
  
    delay(1000);
  }
}

ISR(TIMER1_COMPA_vect){
  if (digitalRead(button) == 1){  
    m = counter++;
    microsec = m * 10;
    if(microsec >= 1000){
      millisec++;
    }
  }
}

#define pwma 9

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(pwma, OUTPUT);
  TCCR1A=(1<<COM1A1) | (0<<COM1A0) | (1<<COM1B1) | (0<<COM1B0) | (1<<WGM11) | (0<<WGM10);
  TCCR1B=(0<<ICNC1) | (0<<ICES1) | (1<<WGM13) | (0<<WGM12) | (1<<CS12) | (0<<CS11) | (0<<CS10);
  ICR1H=0x02;
  ICR1L=0x71; 
}

void loop() {
  if(Serial.available()){
    int val = Serial.read();
    OCR1A = val;
    delay(10);
  }
  
}

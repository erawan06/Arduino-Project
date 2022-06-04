#define buzz 9
#define C5 2093
#define D5 2349
#define E5 2637
#define F5 2790
#define G5 3136
#define A5 3520
#define B5 3951
#define C6 4186

void setup() {
  // put your setup code here, to run once:
  Serial.begin (115200);
  pinMode (buzz, OUTPUT);
  TCCR1A=(0<<COM1A1) | (1<<COM1A0) | (0<<COM1B1) | (1<<COM1B0) | (0<<WGM11) | (0<<WGM10);
  TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (1<<WGM12) | (0<<CS12) | (0<<CS11) | (1<<CS10);
}

void tune(int frequency,int lenght,int pause){
  int countOCR = 16000000/(2*frequency)-1;
  OCR1AH = countOCR >> 8;
  OCR1AL = countOCR & 0xff;
  delay(lenght);
  OCR1AH = 0; OCR1AL = 0;
  delay(pause);
}

void tune_up(int frequency,int lenght,int pause){
  int countOCR = 16000000/(2*frequency);
  ICR1H = (countOCR >> 8) & 0x00FF;
  ICR1L = countOCR;
  unsigned int duty_Cycle = countOCR*50/100;
  OCR1AH = (duty_Cycle >> 8) & 0x00FF;
  OCR1AL = duty_Cycle;
  delay(lenght);
  OCR1A=0;
   delay(pause);
}


void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available()){
    int val = Serial.read();
    if(val == '1'){
      tune(G5,500,20);
      tune(E5,500,20);
      tune(G5,500,20);
      tune(E5,500,20);
      tune(E5,500,20);
      tune(F5,500,20);
      tune(G5,500,500);
      
      tune(F5,500,20);
      tune(D5,500,20);
      tune(F5,500,20);
      tune(A5,500,20);
      tune(G5,500,20);
      tune(F5,500,20);
      tune(E5,500,500);
      
      tune(A5,500,20);
      tune(F5,500,20);
      tune(A5,500,20);
      tune(F5,500,20);
      tune(A5,500,20);
      tune(B5,500,20);
      tune(C6,500,500);
      
      tune(C6,500,500);
     
      tune(E5,500,20);
      tune(G5,500,20);
      tune(F5,500,20);
      tune(D5,500,20);
      tune(C5,500,1000);
      }
      
      if(val == '2'){
      tune_up(G5,500,20);
      tune_up(E5,500,20);
      tune_up(G5,500,20);
      tune_up(E5,500,20);
      tune_up(E5,500,20);
      tune_up(F5,500,20);
      tune_up(G5,500,500);
   
      tune_up(F5,500,20);
      tune_up(D5,500,20);
      tune_up(F5,500,20);
      tune_up(A5,500,20);
      tune_up(G5,500,20);
      tune_up(F5,500,20);
      tune_up(E5,500,500);
  
      tune_up(A5,500,20);
      tune_up(F5,500,20);
      tune_up(A5,500,20);
      tune_up(F5,500,20);
      tune_up(A5,500,20);
      tune_up(B5,500,20);
      tune_up(C6,500,500);
  
      tune_up(C6,500,500);
  
      tune_up(E5,500,20);
      tune_up(G5,500,20);
      tune_up(F5,500,20);
      tune_up(D5,500,20);
      tune_up(C5,500,1000);
      }
 }  
}

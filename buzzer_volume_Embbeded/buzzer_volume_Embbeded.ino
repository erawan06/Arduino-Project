#define C5 2093
#define D5 2349
#define E5 2637
#define F5 2790
#define G5 3136
#define A5 3520
#define B5 3951
#define C6 4186
int musik;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(9, OUTPUT);
  TCCR1A=(1<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<WGM11) | (0<<WGM10);
  TCCR1B=(0<<ICNC1) | (0<<ICES1) | (1<<WGM13) | (0<<WGM12) | (0<<CS12) | (0<<CS11) | (1<<CS10);
}

void tune(int frequency, int lenght, int pause, float volume){
  int TOP=2000000/(2*frequency);
  ICR1=TOP;
  musik=TOP*volume;
  OCR1A=musik;
  Serial.println(OCR1A);
  delay(lenght);
  OCR1A=0;
  delay(pause);
}

void loop() {
  // put your main code here, to run repeatedly:
  tune(G5,500,20,1);
  tune(E5,500,20,2);
  tune(G5,500,20,1);
  tune(E5,500,20,1);
  tune(E5,500,20,1);
  tune(F5,500,20,1);
  tune(G5,500,20,1);
  delay(500);
  tune(F5,500,20,1);
  tune(D5,500,20,1);
  tune(F5,500,20,1);
  tune(A5,500,20,1);
  tune(G5,500,20,1);
  tune(F5,500,20,1);
  tune(E5,500,20,1);
  delay(500);
  tune(A5,500,20,1);
  tune(F5,500,20,1);
  tune(A5,500,20,1);
  tune(F5,500,20,1);
  tune(A5,500,20,1);
  tune(B5,500,20,1);
  tune(C6,500,20,1);
  delay(500);
  tune(C6,500,20,1);
  delay(500);
  tune(E5,500,20,1);
  tune(G5,500,20,1);
  tune(F5,500,20,1);
  tune(D5,500,20,1);
  tune(C5,500,20,1);
  delay(1000);
}

#define LED_1 9
#define LED_2 10

unsigned int i = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  TCCR1A = (1 << COM1A1) | (0 << COM1A0) | (1 << COM1B1) | (0 <<COM1B0) | (1 << WGM11) | (0 << WGM10);
  TCCR1B = (0 << ICNC1) | (0 << ICES1) | (1 << WGM13) | (1 << WGM12) | (0 << CS12) | (1 << CS11) | (0 << CS10);
  ICR1H = 0x07;
  ICR1L = 0xCF;
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.read()== 'u'){
    i += 100;
    OCR1A = i;
    OCR1B = i;
    Serial.print(OCR1A);
    Serial.print(" : ");
    Serial.println(OCR1B);
  }
  if(Serial.read()== 'd'){
    i -= 100;
    OCR1A = i;
    OCR1B = i;
    Serial.print(OCR1A);
    Serial.print(" : ");
    Serial.println(OCR1B);
    if (i <= 0)i = 0;
  }
}

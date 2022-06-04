#define ch_a 2
#define ch_b 3

int counter = 0;
 
void setup() {
  // put your setup code here, to run once:
  pinMode(ch_a,INPUT); pinMode(ch_b,INPUT);
  Serial.begin(9600);

  // EZ MODE
  //attachInterrupt(digitalPinToInterrupt(ch_a), interupt_1 ,CHANGE); 

  // HARD MODE
  // External Interrupt(s) initialization
  // INT0: On
  // INT0 Mode: Any change
  // INT1: Off
  // Interrupt on any change on pins PCINT0-7: Off
  // Interrupt on any change on pins PCINT8-14: Off
  // Interrupt on any change on pins PCINT16-23: Off
  EICRA=(0<<ISC11) | (0<<ISC10) | (0<<ISC01) | (1<<ISC00);
  EIMSK=(0<<INT1) | (1<<INT0);
  EIFR=(0<<INTF1) | (1<<INTF0);
  PCICR=(0<<PCIE2) | (0<<PCIE1) | (0<<PCIE0);
   

}

void loop() {
  // put your main code here, to run repeatedly:
  delay(1000);
}

// EZ MODE
//void interupt_1(){
//  if(digitalRead(ch_a)==digitalRead(ch_b)) counter ++;
//  else counter--;
//  Serial.print("Counter : ");
//  Serial.println(counter);
//}

// HARDMODE
ISR(INT0_vect){  
  if(digitalRead(ch_a)==digitalRead(ch_b)) counter ++;
  else counter--;
  Serial.print("Counter : ");
  Serial.println(counter);
}

//#define Output 8
#define interruptPin 2
volatile byte state = LOW;
int c = 0;

void setup() {
  Serial.begin(115200);
 // pinMode(Output, OUTPUT);
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), blink, FALLING);
}

void loop() {
  //digitalWrite(Output, state);
//  Serial.println(digitalRead(Output));
}

void blink() {
  state = !state;
  c++;
  Serial.println(c);
}

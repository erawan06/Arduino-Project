#define timer 2
#define rotary 3

int counter = 0;
 
void setup() {
  // put your setup code here, to run once:

  pinMode(timer,INPUT);
  pinMode(rotary,INPUT);
  Serial.begin(115200);
  attachInterrupt(digitalPinToInterrupt(timer), inter ,CHANGE); 
}

void loop() {
  // put your main code here, to run repeatedly:

}

void inter(){
  if(digitalRead(timer)==digitalRead(rotary))
    counter ++;
  else
    counter--;
  Serial.println(counter);
}

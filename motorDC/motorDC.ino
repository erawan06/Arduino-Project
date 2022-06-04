const int pwm = D4 ;  //enable motor output 1
const int in_1 = D5 ; // 
const int in_2 = D8 ;// 

const int pwm2 = D3 ;  //enable motor output 2
const int in_11 = D1 ; //
const int in_22 = D2 ;// 

int rainsense = D6; // analog sensor pin A0 untuk input raindrop
static int flag_naik,flag_turun;
//For providing logic to L298 IC to choose the direction of the DC motor 

void setup()
{
pinMode(pwm,OUTPUT) ;   //we have to set PWM pin as output
pinMode(in_1,OUTPUT) ;  //Logic pins are also set as output
pinMode(in_2,OUTPUT) ;
pinMode(pwm2,OUTPUT) ;   //we have to set PWM pin as output
pinMode(in_11,OUTPUT) ;  //Logic pins are also set as output
pinMode(in_22,OUTPUT) ;
Serial.begin(115200);
}


void loop()
{
//For Clock wise motion , in_1 = High , in_2 = Low

int rainSenseReading = analogRead(rainsense); 
Serial.println(rainSenseReading);    
  delay(250);// hujan value dari 0 ke 1023.
  if(rainSenseReading <1023){
   // sendingData(nodeclient1);
    Serial.println("Hujan");
    if(flag_naik == 1){
      //For brake
      digitalWrite(in_1,HIGH) ;
      digitalWrite(in_2,HIGH) ;
      digitalWrite(in_11,HIGH) ;
      digitalWrite(in_22,HIGH) ;
      delay(1000) ;
      Serial.println("Sek Hujan");
      }
    else { 
      digitalWrite(in_1,LOW) ; // bismillah iki paten
      digitalWrite(in_2,HIGH) ;
      digitalWrite(in_11,LOW) ;
      digitalWrite(in_22,HIGH) ;
      analogWrite(pwm,5000) ;
      analogWrite(pwm2,5000) ;
      delay(30);
      } 
    flag_naik = 1;  
    flag_turun = 0;
   }
 else {
  Serial.println("tidak hujan");
  if(flag_turun == 1){
      //For brake
      digitalWrite(in_1,HIGH) ;
      digitalWrite(in_2,HIGH) ;
      digitalWrite(in_11,HIGH) ;
      digitalWrite(in_22,HIGH) ;
      delay(1000) ;
      Serial.println("Brake");
      }
    else { 
      digitalWrite(in_1,HIGH) ;
      digitalWrite(in_2,LOW) ;
      digitalWrite(in_11,HIGH) ;
      digitalWrite(in_22,LOW) ;
      analogWrite(pwm,5000) ;
      analogWrite(pwm2,5000) ;
      delay(30);
      Serial.println("aku turun");
      }
  flag_naik = 0;
  flag_turun = 1;
 }
//analogWrite(pwm,300) ;

/*setting pwm of the motor to 255
we can change the speed of rotaion
by chaning pwm input but we are only
using arduino so we are using higest
value to driver the motor  */

//Clockwise for 3 secs


//For Anti Clock-wise motion - IN_1 = LOW , IN_2 = HIGH
//digitalWrite(in_1,LOW) ;
//digitalWrite(in_2,HIGH) ;
////analogWrite(pwm,300) ;
//delay(300) ;
//
////For brake
//digitalWrite(in_1,HIGH) ;
//digitalWrite(in_2,HIGH) ;
//delay(3000) ;
 }

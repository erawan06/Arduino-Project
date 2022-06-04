//library yang digunakan
#include <LiquidCrystal.h>
#include <CaptureTimer.h>
#include <Encoder.h>
#include <Time.h>
//Pin yang digunakan 
#define ENCODER_OPTIMIZE_INTERRUPTS
#define dirMotor  2
#define pwmMotor  3
#define EncodIn  26                     // Encoder Input 
#define ctINPin      52 
#define samplingPer    20
#define tickOUTPin    4   // Pin used as tick generator (for standalone board testing purposes)// Literal Pin / Board
//Pin encoder motor dan rotary
Encoder encModule(24,25);
Encoder encMotor(50,52);
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
//variabel untuk memilih hasiloutput untuk MFC,serialMonitor, atau Serial Plotter
char inputmod,mode=2;
int i=0;
//Variabel lcd
int lcd_key = 0;
//variabel untuk menggerakkan encoder motor dan rotry encoder
long positionModule=0;
long positionMotor=0;
//variabel untuk menghitung RPM dari nilai frekuensi dan periode,Duty cycle
int pwm=0;
float frekuensi=0;
unsigned long simpan=0,putar;
float rpmf=0,periode,rpmt;
float dutyC=0;
//variabel untuk menampilkan tegangan dari sensor tegangan
float Voltage = 0;
float Current = 0;
float vRead = 0;
float R1 = 30000;
float R2 = 3300;
float iRead = 0;
int mVperAmp = 185;
int ACSoffset = 1650;
float vCalc=0;
int b=0;
static int flag = 0;
int dataRand;
int counter=0;
char inByte;
unsigned long prevmillis=0;
void cariFreq()
{
    uint16_t ticks;
    (void) CaptureTimer::getFreq(&ticks);
    frekuensi=ticks;
}
void dutC()
{
  dutyC=pwm/255*100;
}
void periodes()
{
  uint32_t Time;
  if (CaptureTimer::getTickCapture(&Time) == true)
  { // Only print on serial if new data is available
    periode = 1/frekuensi;
    rpmt = 60 / (periode * 200);
  }
}
void serialPlotter()
{
 // Serial.print("DATA PWM= ");
  Serial.print(pwm);
  Serial.print(" ");
    //Serial.print("DATA RPM= ");
  Serial.print(rpmf);
  Serial.print(" ");
  Serial.print(rpmt);
  Serial.print(" ");
  Serial.println(Voltage);
}
void serialPrint()
{
 Serial.print("DATA PWM= ");
 Serial.print(pwm);
 Serial.print(" ");
 Serial.print("Duty Cycle= ");
 Serial.print(dutyC);
 Serial.print("% ");
 Serial.print("RPM(Freq)= ");
 Serial.print(rpmf);
 Serial.print(" ");
 Serial.print("RPM(P)= ");
 Serial.print(rpmt);
 Serial.print(" ");
 Serial.print("DATA VOLTAGE= ");
 Serial.println(Voltage);
 Serial.print(" ");
  delay(500);
}
void serialMFC()
{
  Serial.print('#');
  Serial.print(pwm);
  Serial.print('$');
  Serial.print(rpmf);
  Serial.print('&');
  Serial.print(rpmt);
  Serial.print('%');
  Serial.print(Voltage);
  Serial.println('!');
}
void voltage()
{
    iRead = analogRead(11) * 3300 / 1024;
  Current = ((iRead-ACSoffset) / mVperAmp) * 1000;
  vRead = analogRead(10) * 3.3 / 1024;
  Voltage = vRead / (R2 / (R1+R2));
}

void ReadRPM(float inputfreq)
{
unsigned  long newEncMotor,positionMotor;
  //cu_time=millis();
     newEncMotor = encMotor.read();
   if (newEncMotor != positionMotor)
   {
   
      positionMotor = newEncMotor;
        
   }
  //Frekuensi
  rpmf=inputfreq*60/200; // nilai 200 berdasarkan 1 kali perputaran
   lcd.setCursor(9,1);
      lcd.print(rpmf);
  
 
}
void setup() {
    Serial.begin(115200);
  // initialize the capture timer pin & period
  CaptureTimer::initCapTicks(samplingPer, ctINPin);
  CaptureTimer::initCapTime(ctINPin);
  lcd.begin(16,2);
  lcd.setCursor(0,0);         // Set posisi 0,0 lcd 
  lcd.print("ENCODER: ");
  lcd.setCursor(0,1);         // Set posisi 0,1 lcd 
  lcd.print("RPM    : ");
  pinMode(EncodIn,INPUT);      // Initialize Mode input encoder;
  pinMode(dirMotor, OUTPUT);
  pinMode(pwmMotor, OUTPUT);
    pinMode(tickOUTPin, OUTPUT);
}
void loop() 
{
   if (Serial.available()) {
    inByte = Serial.read();
        if(inByte == '1') flag = 1;
    else if(inByte == '2')  flag = 2;
  }
  if (flag == 1){

    dataRand=rand()%255;
    //positionModule = newEncModule;

     if(dataRand<=124)
        dataRand=0;
     else if(dataRand>124)
        dataRand=255;
        
      cariFreq();
     ReadRPM(frekuensi);
     digitalWrite(dirMotor,LOW);
     analogWrite(pwmMotor,dataRand);
     Serial.print(dataRand);
     Serial.print(" ");
     Serial.println((int)rpmf);
     counter++;
     if (counter == 127){
      Serial.println(" DONE");
      flag = 2;
           
     
  }
  delay(40);
 }
}

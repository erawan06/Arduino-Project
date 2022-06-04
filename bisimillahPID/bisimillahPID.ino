#include <LiquidCrystal.h>
#include <Encoder.h>
#include <CaptureTimer.h>
#define tickOUTPin      4 
#define ctINPin         52 
#define timeSampling    10
#define dirMotor        2
#define pwmMotor        3 

#define SERIAL          1
#define PLOT            2
#define MFC             3
#define ENCODER_OPTIMIZE_INTERRUPS
#define RPM_MAX         4000

float rpmp, periode, freq, rpmf;
Encoder encModule(24,25);
Encoder encMotor(50,52);
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
float Voltage = 0;
float vRead = 0;
float R1  = 30000;
float R2  = 3300;
int counter = 0;
float duty;
static int mode;
char inbyte;

long positionModule = 0;
long positionMotor = -999;
long newEncModule,newEncMotor;

int PulsePerRevolution = 200;
float tempEncModule = 0;
float Proportional, Integral, Derivative, PID;


void tickGenerator()
{
  // Note that this tick generator stretches following the amount of code runned by cpu cycle & interrupts
  // use a real freq generator of any kind for accurate tick(s) if you want to test library results accuracy
  static const uint16_t ovf = ((uint16_t) -1L) / 10;  // overflow at 10% of full cnt scale
  static const uint16_t thr = ovf / 100;        // threshold at 1% of overflow cnt scale
  static uint16_t cnt = 0;
  
  digitalWrite(tickOUTPin, (cnt >= thr) ? HIGH : LOW);
  if (++cnt > ovf)  { cnt = 0; }
}

void tickGeneratorPeriode()
{
  // Note that this tick generator stretches following the amount of code runned by cpu cycle & interrupts
  // use a real freq generator of any kind for accurate tick(s) if you want to test library results accuracy
  static const uint16_t ovf2 = ((uint16_t) -1L) / 10; // overflow at 100% of full cnt scale
  static const uint16_t thr2 = ovf2 / 100;        // threshold at 50% of overflow cnt scale
  static uint16_t cnt2 = 0;
  
  digitalWrite(tickOUTPin, (cnt2 >= thr2) ? HIGH : LOW);
  if (++cnt2 > ovf2)  { cnt2 = 0; }
  
  if (cnt2 == thr2)
  {
    micros();
    CaptureTimer::startTickCapture();
  }
}

void printSerial(){

  Serial.print("Set Point = "); Serial.print(tempEncModule);
  Serial.print(" PV RPM = "); Serial.print(rpmf);
  Serial.print(" PWM PID[0 - 255]= "); Serial.print(PID);
  Serial.print(" Error = "); Serial.println(tempEncModule - rpmf);
  if(counter >= 20000){
      Serial.println("done");
     }
}

void printPlot(){
  Serial.print((int)rpmf);
  Serial.print(" ");
  Serial.print(tempEncModule);
  Serial.print(" ");
  Serial.println(" ");  
}

void printMFC() {
 Serial.print((int)rpmf);           // rpmf
 Serial.print('_');
 Serial.print((int)tempEncModule);  // set point RPM
 Serial.print('_');
 Serial.println(" ");
 
 // Serial.print('\n');  
}

void setup() {
  Serial.begin(115200);
  // put your setup code here, to run once:
  lcd.begin(16,2);
  lcd.setCursor(0,0);
  lcd.print("ENC   = ");
  lcd.setCursor(0,1);
  lcd.print("RPM   = ");

  // initialize the capture timer pin & period
  CaptureTimer::initCapTicks(timeSampling, ctINPin);
  CaptureTimer::initCapTime(ctINPin);
  
  pinMode(tickOUTPin, OUTPUT);
}

void RPMPeriod()
{
  uint32_t Time;
  tickGeneratorPeriode();
  
  if (CaptureTimer::getTickCapture(&Time) == true)
  { 
    periode = (float)Time / 1000000;
    rpmp = 60 / (periode * PulsePerRevolution);
  }

}

void RPMFreq()
{
  uint16_t ticks;  
  // tickGenerator();

  (void)CaptureTimer::getFreq(&ticks);
  freq = ticks;
  rpmf = freq * 60 / PulsePerRevolution;     
}

void SetSpeedPWM()
{
    newEncModule = encModule.read();
    if (newEncModule != positionModule)
    {
      if (newEncModule >= 255)  newEncModule = 255;
      else if(newEncModule <=0) newEncModule = 0;
          
      lcd.setCursor(9,0);
      lcd.print(newEncModule);
      
      // hitung duty cycle
      duty = (float)newEncModule / 255 * 100;          
      positionModule = newEncModule;
    }

    tempEncModule = (float)newEncModule/255 * RPM_MAX;
          
//    digitalWrite (dirMotor, LOW);
//    analogWrite (pwmMotor, newEncModule);
}



void SetPID()
{
  // Variable PID
  static float Err = 0, LastErr = 0; 
  static float IntegralErr = 0, DerivativeErr = 0;
  //static float KP = 0.0029;
  //static float KI = 0.4;
  //static float KD = 0;
  static float KP = 0.065576;
  static float KI = 2.1239;
  static float KD = 0.000047161;
  static float TimeSampling = 0.01; // 10 ms
      
  float SetPoint = tempEncModule;
  
  Err = SetPoint - rpmf;
  IntegralErr += (Err * TimeSampling);
  
  // Set constraint
  if (IntegralErr >= 1000)      IntegralErr = 1000;
  else if(IntegralErr <= -1000) IntegralErr = -1000;
  DerivativeErr = (Err - LastErr) / TimeSampling;

  LastErr = Err;

  Proportional = Err * KP;
  Integral = IntegralErr * KI;
  Derivative = DerivativeErr * KD;
  
  PID = Proportional + Integral + Derivative;
 
  if (PID > 255)    PID = 255;
  else if (PID < 0) PID = 0;

  digitalWrite (dirMotor, LOW);
  analogWrite (pwmMotor, PID);
}

void GetVoltage()
{
  vRead  = analogRead(10) * 3.3 /1024;
  Voltage = vRead / (R2 / (R1+R2));
}

void GetInput()
{
  if(Serial.available()) 
  {
    inbyte = Serial.read();
    if (inbyte == '1')      mode = SERIAL;
    else if(inbyte == '2')  mode = PLOT;
    else if(inbyte == '3')  mode = MFC;
  }

  switch(mode) {
    case SERIAL: printSerial();     break;
    case PLOT:   printPlot(); break;
    case MFC:    printMFC();  break;
    default:     printPlot(); break;
  }  
}

void loop() 
{
  // put your main code here, to run repeatedly:  
  
  RPMFreq();
  RPMPeriod();
  // GetVoltage();
  GetInput();          
  
  SetSpeedPWM();
  SetPID();
  
  delay(10);
}

//#include <CaptureTimer.h>
//  #define ctINPin     52 
//  #define samplingPer   10
//  #define ENCODER_OPTIMIZE_INTERRUPS
//  #define gndMotor 2
//  #define pwmMotor 3 
//int random2; 
//char inbyte;
//int mode = 2;
//int counter = 0;
//float freq, rpmf;
//unsigned long Prevmillis=0;
//void frekuensi(){ 
//  uint16_t ticks;
//  (void) CaptureTimer::getFreq(&ticks);
//  freq = ticks;
//  rpmf = freq * 60 / 200;
//   
//}
//
// 
//void setup() {
//  Serial.begin(115200);
//  CaptureTimer::initCapTicks(samplingPer, ctINPin);
//}
//
//void loop() {
//
//  if(Serial.available()){
//        inbyte = Serial.read();
//         if (inbyte == '1'){
//           mode = 1;
//         }else if(inbyte == '2'){
//           mode = 2;
//         }
//  }
//  
//  if (mode == 1){
//    random2 = rand()% 255;
//     if (random2 <= 124){
//        random2 = 0;
//     }else{
//        random2 = 255;
//     }
//     
//     
//     frekuensi();
//     digitalWrite (gndMotor, LOW);
//     analogWrite (pwmMotor, random2); 
//     
//     Serial.print(int(random2));
//     Serial.print(" ");
//     Serial.println(int(rpmf));
//
//       counter++;
//     if(counter >= 20000){
//      Serial.println("done");
//      mode = 2;
//      
//     }
//     delay(10);
//    
//     
//   
//  }else{
//     digitalWrite (gndMotor, LOW);
//     digitalWrite (pwmMotor, LOW); 
//  }
//}

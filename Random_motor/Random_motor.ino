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
#define SAMPLE_MAX      2000

float rpmp, periode, freq, rpmf;
Encoder encModule(24,25);
Encoder encMotor(50,52);
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
float Voltage = 0;
float vRead = 0;
float R1  = 30000;
float R2  = 3300;
float duty;
static int mode;
char inbyte;

long positionModule = 0;
long positionMotor = -999;
long newEncModule,newEncMotor;

int PulsePerRevolution = 200;
float tempEncModule = 0;
float Proportional, Integral, Derivative, PID;

int varRandom[SAMPLE_MAX] = {0};
void getRandom()
{
  int i,tambah = 0, threshold=127, data=0;
  for(i=0; i<SAMPLE_MAX; i++)
  {
    RPMFreq();  
    data = rand() % 255;
    if (data > threshold) data = 255;
    else                  data = 0;      
    
    digitalWrite (dirMotor, LOW);
    analogWrite (pwmMotor, data);
    
    Serial.print(data);//data random
    Serial.print(" ");
    Serial.println(rpmf);//rpm
    tambah++;
    if (i == 1999) delay(10000);
    
    delay(10);
  }

  Serial.println("END RNDM");
}

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
  Serial.print(" Error = "); Serial.print(tempEncModule - rpmf);
  Serial.print(" Volt = "); Serial.println(Voltage);

  
//  Serial.print("PWM = ");
//  Serial.print(newEncModule);
//  Serial.print(" | Duty Cycle = ");
//  Serial.print(duty);
//  Serial.print(" | RPMF = ");
//  Serial.print((int)rpmf);
//  Serial.print(" | RPMP = ");
//  Serial.print((int)rpmp);
//  Serial.print(" | TEGANGAN = ");
//  Serial.print(Voltage);
//  Serial.println(' ');
  
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
 Serial.print(' ');
 Serial.print((int)tempEncModule);  // set point RPM
 Serial.print(' ');
  Serial.println(" "); 
// Serial.print((int)PID);          // [0 - 255]
// Serial.print('$');
 
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
  tickGenerator();

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
  static float KP = 0.12;
  static float KI = 0.4;
  static float KD = 0.00005;
  static float TimeSampling = 0.01; // 10 ms
      
  // SP = RPM (0 - 4000);
  // PV = RPM (0 - 4000);
  //  int SetPoint = 2000;  // IN RPM
  float SetPoint = tempEncModule;
  
  Err = SetPoint - rpmf;
  IntegralErr += (Err * TimeSampling);

//  Serial.print("Set Point = "); Serial.print(SetPoint);
//  Serial.print("PV RPM = "); Serial.print(rpmf);
//  Serial.print("Error = "); Serial.println(Err);
      
//  Serial.print("Error: ");  Serial.print(Err);
//  Serial.print(" | IntErr: "); Serial.println(IntegralErr);
  
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
//  Serial.print(" PWM PID[0 - 255]= "); Serial.println(PID);

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
  GetVoltage();
  GetInput();          
  
  SetSpeedPWM();
  getRandom();
  // SetPID();
  
  delay(10);
}

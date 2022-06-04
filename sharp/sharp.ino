#define sensorIR A0     //definisi pin analog
float sensorValue, inches, mm, cm;     

void setup() {
Serial.begin(9600);
}

void loop() {
sensorValue = analogRead(sensorIR);    //baca data analog

//Rumus Sensor Sharp GP2Y0A02YK0F_____________________
inches = 4192.936 * pow(sensorValue,-0.935) - 3.937;
mm = 10650.08 * pow(sensorValue,-0.935) - 10;
cm = mm/10;
//____________________________________________________

delay(100);
Serial.print("Inches: ");
Serial.print(inches);
Serial.print(" | Cm: ");
Serial.println(cm);
}

#include <SoftwareSerial.h>
#include "DualG2HighPowerMotorShield.h"
#include <Adafruit_SCD30.h>

DualG2HighPowerMotorShield18v22 md;
Adafruit_SCD30 scd30;



 unsigned long Actual;
 unsigned long last = 0;
 unsigned long Delay = 5000;

void sensor(){

if (!scd30.begin()) {
    Serial.println("Failed to find sensor, trying again"); }

  if (scd30.dataReady()) {
    scd30.read();
    Serial.println("Measurement interval: " + String(scd30.getMeasurementInterval()) + " seconds");

    Serial.print("CO2: ");
    Serial.print(scd30.CO2, 1);
    Serial.print(" ppm, ");
    
    Serial.print("Temperature: ");
    Serial.print(scd30.temperature, 1);
    Serial.print(" °C, ");
    
    Serial.print("Humidity: ");
    Serial.print(scd30.relative_humidity, 1);
    Serial.println(" %");
    
    Serial.println("----------------------");
  }

}




void setup() {
  Serial.begin(115200);
  md.calibrateCurrentOffsets();
 
  delay(10);
  
  md.enableDrivers();
  md.flipM2(true);
  Serial.println("Conexion establecida");
  Serial.println("Sensor test"); delay(20);
  if (!scd30.begin()) {
    Serial.println("Failed to find sensor, trying again"); } 
  else{ Serial.println("SENSOR FOUND");}
  delay(2000); 
  sensor();
  
}

void Avanzar(){
//stopIfFault();
md.setM1Speed(400);
md.setM2Speed(400);
}

void Detener(){
// stopIfFault();
md.setM1Speed(0);
md.setM2Speed(0);
}


void Retroceder() {
// stopIfFault();
md.setM1Speed(-400);
md.setM2Speed(-400);
}

void Derecha() {
//stopIfFault();
md.setM1Speed(-400);
md.setM2Speed(400);
}


void Izquierda(){
  //stopIfFault();
  
md.setM1Speed(400);
md.setM2Speed(-400);
}

void loop() {
 Actual = millis();

  if(Actual - last >= Delay){
     last = Actual;
      sensor();
    } 
 
  if (Serial.available() > 0){
    char currentCommand = Serial.read();
   
  

  
    switch(currentCommand){

      case 'w':
      Avanzar();
      break;

      case 's':
      Retroceder();
      break;

      case 'd':
      Derecha();
      break;

      case 'a':
      Izquierda();     
      break;

      case 'p':
      Detener();
      break;

      case 'h':
      md.enableDrivers();
      break;
    
    }
  }
}

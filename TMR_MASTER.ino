


     
  // 102, 235, 370 cuello
//Brazo 1: 369 - 200    369 es minimo
// 245-430 servo AESIR

#include <SoftwareSerial.h>
#include "DualG2HighPowerMotorShield.h"
#include <Adafruit_SCD30.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pca9685 = Adafruit_PWMServoDriver();
DualG2HighPowerMotorShield18v22 md;
Adafruit_SCD30 scd30; 

class Temporizador {
  private:
    unsigned long tiempo;
    unsigned long inicial = millis();

  public:
    Temporizador(unsigned long tiempo) {
      this->tiempo = tiempo;
      inicial = millis();
    } 

    long faltante() {
      return tiempo + inicial - millis();
    }

    bool termino() {
      return faltante() <= 0;
    }

    void reiniciar() {
      inicial = millis();
    }

    void cambiar(long nuevo) {
      tiempo = nuevo;
    }
};

class ServoControl{
  private:
    static Adafruit_PWMServoDriver pca9685;
    
    unsigned int canal;
    unsigned long intervalo;
    Temporizador timer = NULL;
    
  public:
  unsigned int MAX = 375; //no se usan
  unsigned int MIN = 102; //no se usan
  unsigned int pulso;
  unsigned int actual;
  ServoControl(unsigned int canal, unsigned int pulso, unsigned long intervalo){
    this->canal = canal;
    this->pulso = pulso;
    this->actual = pulso;
    timer = Temporizador(intervalo);
  }

  void inciar(unsigned long inicio){
    pca9685.setPWM(canal, 0, inicio); 
    pulso = inicio;
  }
 
  static void iniciardriver() {
      pca9685.begin();
      pca9685.setPWMFreq(50);
    }
 
  void stop() {
      pulso = actual;
    }

    void update() {
      if (actual == pulso) return;
      if (!timer.termino()) return;
      timer.reiniciar();
      if (actual > pulso) {actual = actual- 3;}
      else actual = actual + 3;

      pca9685.setPWM(canal, 0, actual);
    }

    void update_absoluto(){
      pca9685.setPWM(canal, 0, pulso);
    }


   void cambiar_pulso(unsigned int nuevo_pulso){
    pulso = nuevo_pulso;
  }

  void cambiar_velocidad( unsigned long nueva_velocidad){
    intervalo = nueva_velocidad;
  } 

};


void sensor(){
if (scd30.dataReady()){
    Serial.println("Data available");

    if (!scd30.read()){ Serial.println("Error reading sensor data"); return; }

    //Serial.println("Measurement interval: " + String(scd30.getMeasurementInterval()) + " seconds");
     
    Serial.print("Temperature: ");
    Serial.print(scd30.temperature, 1);
    Serial.print(" °C, ");
    Serial.print("CO2: ");
    Serial.print(scd30.CO2, 1);
    Serial.print(" ppm, ");
     Serial.print("Humidity: ");
    Serial.print(scd30.relative_humidity, 1);
    Serial.println(" %");
    
    Serial.println("----------------------");
  } 

}


void Avanzar(){
//stopIfFault();
md.setM1Speed(-400);
md.setM2Speed(-400);
}

void Detener(){
// stopIfFault();
md.setM1Speed(0);
md.setM2Speed(0);
}


void Retroceder() {
// stopIfFault();
md.setM1Speed(400);
md.setM2Speed(400);
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



unsigned int MAX1 = 375;
unsigned int MIN1 = 102;


Adafruit_PWMServoDriver ServoControl::pca9685;
ServoControl back1 = ServoControl(0, 228, 15);
ServoControl back2 = ServoControl(1, 228, 15);
ServoControl front1 = ServoControl(2, 228, 15);
ServoControl front2 = ServoControl(3, 228, 15);
ServoControl base = ServoControl(4, 390,20);
ServoControl codo = ServoControl(5, 155,20);
ServoControl cuello = ServoControl(6, 235, 20);
ServoControl garra = ServoControl(7, 102, 15);

int cont;
void setup() {
  Serial.begin(115200);
  Serial.setTimeout(10);
  
  md.calibrateCurrentOffsets();
  delay(10);
  
  md.enableDrivers();
  
  md.flipM2(true);

  
  ServoControl::iniciardriver();
 
  Serial.println("Conexion establecida");
  back1 = ServoControl(0, 228, 15);
  back2 = ServoControl(1, 228, 15);
  front1 = ServoControl(2, 228, 15);
  front2 = ServoControl(3, 228, 15);
  base = ServoControl(4, 390,20);
  codo = ServoControl(5, 155, 20);
  cuello = ServoControl(6, 235, 20);
  garra = ServoControl(7, 102, 15);
  //int cont = 0;
  back1.inciar(228);//arriba
  back2.inciar(228);//arriba
  front1.inciar(228);
  front2.inciar(228); //arriba
  base.inciar(390);
  codo.inciar(155);
  cuello.inciar(235);
  garra.inciar(102);
  
  Serial.println("Sensor test"); delay(20);
  if (!scd30.begin()) {
    Serial.println("Failed to find sensor"); } 
  else{ Serial.println("SENSOR FOUND");}
  delay(500); 
  sensor();
 
}

char lastCommand = '\0';
Temporizador co2_timer = Temporizador(5000);
Temporizador regresar = Temporizador(1000);
//Temporizador PosInicial = Temporizador(500);
void loop() {
codo.update();
back1.update();
back2.update();
front2.update();
front1.update();
base.update();
cuello.update();
garra.update();
 
 if(co2_timer.termino()){
  co2_timer.reiniciar();
  sensor();
 }
 
  while (Serial.available() > 0){
    char currentCommand = Serial.read();
   
    if(currentCommand != lastCommand){
      lastCommand = currentCommand;


  
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

        case 'b':
        Detener();
        back1.stop();
        back2.stop();
        front2.stop();
        front1.stop();
        base.stop();
        codo.stop();
        cuello.stop();
        garra.stop();
        break;

        case 'm':
        md.enableDrivers();
        break;

        case 'v'://abajo
        back1.cambiar_pulso(MAX1);
        back2.cambiar_pulso(MIN1);
        break;

        case 'c'://arriba
        back1.cambiar_pulso(MIN1);
        back2.cambiar_pulso(MAX1);
        break;

        case 't'://arriba
        front2.cambiar_pulso(MIN1); //no cambiar
        break;

        
        case 'g'://abajo
        front2.cambiar_pulso(MAX1);
        break;

        case 'y'://arriba
        front1.cambiar_pulso(MAX1); //no cambiar
        break;

        
        case 'h'://abajo
        front1.cambiar_pulso(MIN1);  
        break;

        case 'z':
        front1.cambiar_pulso(MAX1);
        front2.cambiar_pulso(MIN1);
        break;

        case 'x':
        front1.cambiar_pulso(MIN1);
        front2.cambiar_pulso(357);
        break;

        case 'q':
        front1.cambiar_pulso(357);
        front2.cambiar_pulso(102);
        back1.cambiar_pulso(102);       
        back2.cambiar_pulso(357);
        break;
        
        case 'n':
        if(codo.pulso >  330) { 
        codo.cambiar_pulso(155);
        base.cambiar_pulso(390);  }     
        break;
       
        case 'u':ht
        base.cambiar_pulso(210);
        codo.cambiar_pulso(350);
        break;
        
        case 'i':
        codo.cambiar_pulso(400);
        break;

        case 'k':
        codo.cambiar_pulso(300);
        break;

        case 'j':
        cuello.cambiar_pulso(102);
        break;

        case 'l':
        cuello.cambiar_pulso(370);
        break;

        case 'o':
        garra.cambiar_pulso(330);
        break;

        case 'p':
        garra.cambiar_pulso(102);
        break;

    
    }     //if (currentCommand == 'b') {
      //lastCommand = '\0';}

    }
  }
}



 

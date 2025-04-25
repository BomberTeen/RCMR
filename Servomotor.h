#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pca9685 = Adafruit_PWMServoDriver();
 

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
  unsigned int MAX = 512;
  unsigned int MIN = 102;
  unsigned int pulso;
  unsigned int actual;
  ServoControl(unsigned int canal, unsigned int pulso, unsigned long intervalo){
    this->canal = canal;
    this->pulso = pulso;
    this->actual = pulso;
    timer = Temporizador(intervalo);
  }

  void inciar(){
    pca9685.setPWM(canal, 0, MIN); 
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


   void cambiar_pulso(unsigned int nuevo_pulso){
    pulso = nuevo_pulso;
  }

  void cambiar_velocidad( unsigned long nueva_velocidad){
    intervalo = nueva_velocidad;
  }

};
unsigned int MAX1 = 512;
unsigned int MIN1 = 102;

Adafruit_PWMServoDriver ServoControl::pca9685;
ServoControl a = ServoControl(0, 102, 20);
ServoControl b = ServoControl(1, 102, 20);

void setup(){
  Serial.begin(9600);
  Serial.println("Conexion establecida");

  ServoControl::iniciardriver();
  a = ServoControl(0, 102, 20);
  b = ServoControl(1, 102, 20);
  a.inciar();
  b.inciar();
  //b.cambiar_velocidad(40);
  //delay(5000);
}
void loop(){
  a.update();
  b.update();
  
  if (Serial.available() > 0){
    char currentCommand = Serial.read();
   
    switch(currentCommand){

    case 'z':
    a.cambiar_pulso(MAX1);
    break;

    case 'x':
    a.cambiar_pulso(MIN1);
    break;

    case 'c':
    b.cambiar_pulso(MIN1);
    break;

    case 'v':
    b.cambiar_pulso(MAX1);
    break;

    case 'p':
    b.stop();
    a.stop();
    break;
    }

   
  }
 
}

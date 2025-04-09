//Librerias utilizadas
#include <SoftwareSerial.h>
#include <Servo.h>
#include "Servomotor.h"

//Llantas / Motores
// linea izquierda
int motor1a = 2;
int motor1b = 3;
// linea derecha
int motor2a = 5;
int motor2b = 4;
//Velocidades
int V1 = 9; 
int V2 = 10;

Servomotor brazo();
Servomotor rotor();
Servomotor garra();
Servomotor codo();



void setup() {
  Serial.begin(115200);
  brazo.attach(8);
  codo.attach(11);
  rotor.attach(6);
  garra.attach(7);
  pinMode(motor1a, OUTPUT);
  pinMode(motor2a, OUTPUT);
  pinMode(motor1b, OUTPUT);
  pinMode(motor2b, OUTPUT);

  digitalWrite(motor1a, LOW);
  digitalWrite(motor2a, LOW);
  digitalWrite(motor1b, LOW);
  digitalWrite(motor2b, LOW);


  Serial.println("Conexion establecida");
}
//Definimos las dirreciones del Robot como funciones
void Avanzar(){
  analogWrite(V1,255);
  analogWrite(V2,255);
  digitalWrite(motor1a,HIGH);
  digitalWrite(motor2a,HIGH);
  digitalWrite(motor1b,LOW);
  digitalWrite(motor2b,LOW);
}

void Detener(){
  analogWrite(V1,0);
  analogWrite(V2,0);
  digitalWrite(motor1a,0);
  digitalWrite(motor2a,0);
  digitalWrite(motor1b,0);
  digitalWrite(motor2b,0);
}

void Retroceder() {
    analogWrite(V1,255);
  analogWrite(V2,255);
  digitalWrite(motor1a,LOW);
  digitalWrite(motor2a,LOW);
  digitalWrite(motor1b,HIGH);
  digitalWrite(motor2b,HIGH);

}
void Derecha() {
  analogWrite(V1,255);
  analogWrite(V2,255);
  digitalWrite(motor1a,HIGH);
  digitalWrite(motor2a,LOW);
  digitalWrite(motor1b,LOW);
  digitalWrite(motor2b,HIGH);
}


void Izquierda(){
    analogWrite(V1,255);
  analogWrite(V2,255);
  digitalWrite(motor1a,LOW);
  digitalWrite(motor2a,HIGH);
  digitalWrite(motor1b,HIGH);
  digitalWrite(motor2b,LOW);
}


void loop() {
  //Comprobamos que la comunicacion Serial funcione
  if (Serial.available() > 0){
    char currentCommand = Serial.read();
    Serial.println(currentCommand);

  

    /*Con switch podemos cambiar y usar facilmente entre
    las funciones que declaramos anteriormente
    para el movimiento de los servos */
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

    
      case '1'://"Rest"
      brazo.SetPosition(180);
      codo.SetPosition(30);
      break;

      case '2'://"Reset"
      brazo.SetPosition(90);
      codo.SetPosition(90);
      break;

      case 51: //3 \ 51
      rotor.disminuir();
      break;

      case 52://4  \ 52
      rotor.aumentar();
      break;

      case '5':
      codo.disminuir();
      break;

      case '6':
      codo.aumentar();
      break;

      case '7':
      brazo.disminuir();
      break;

      case '8':
      brazo.aumentar();
      break;
    
   
    //
      case 'c': //c
      garra.abrir();
      break;

      case 'v':
      garra.cerrar();
      break;

      case 't': //Probar los contadores
      Serial.println(last);
      break;
      
      default:
      Detener();
      break;

      case 'p':
      Detener();
      break;

    }
  }
} 
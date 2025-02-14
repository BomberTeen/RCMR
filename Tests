#include <SoftwareSerial.h>
#include <Servo.h>

// Izquierda
int Motor1A = 2;
int Motor1B = 3;

// Derecha
int Motor2B = 4;
int Motor2A = 5;

// Servos
int pos = 90;
int pos2 = 90;

int Prepos1 = 90;
int Prepos2 = 90;

Servo servo1;
Servo servo2;

void setup() {
  // CONEXIÓN SERIAL
  Serial.begin(115200);

  // MOTORES
  pinMode(Motor1A, OUTPUT);
  pinMode(Motor2A, OUTPUT);
  pinMode(Motor1B, OUTPUT);
  pinMode(Motor2B, OUTPUT);

  digitalWrite(Motor1A, LOW);
  digitalWrite(Motor2A, LOW);
  digitalWrite(Motor1B, LOW);
  digitalWrite(Motor2B, LOW);

  // SERVOS
  servo1.attach(9);
  servo2.attach(10);

  Serial.println("Conexion establecida");
}

void loop() {
  if (Serial.available() > 0) {
    char currentCommand = Serial.read();
    Serial.println(currentCommand);

    switch (currentCommand) {
      case 'w': // Avanzar
        digitalWrite(Motor1A, HIGH);
        analogWrite(Motor1B, 0);
        analogWrite(Motor2B, 0);
        digitalWrite(Motor2A, HIGH);
        break;

      case 'r': // Retroceder
        digitalWrite(Motor1B, HIGH);
        analogWrite(Motor1A, 0);
        analogWrite(Motor2A, 0);
        digitalWrite(Motor2B, HIGH);
        break;

      case 'a': // Left
        digitalWrite(Motor2A, HIGH);
        analogWrite(Motor1A, 0);
        analogWrite(Motor2B, 0);
        digitalWrite(Motor1B, HIGH);
        break;

      case 'd': // Right
        digitalWrite(Motor1A, HIGH);
        analogWrite(Motor1B, 0);
        analogWrite(Motor2A, 0);
        digitalWrite(Motor2B, HIGH);
        break;

      case 's': // Stop
        digitalWrite(Motor1A, LOW);
        analogWrite(Motor1B, 0);
        analogWrite(Motor2A, 0);
        digitalWrite(Motor2B, LOW);
        break;

      case '1': //Preposicion  Descanso
        if (currentCommand == '0'){
        Prepos1 = 30;
        Prepos2 = 180;

        while (pos != Prepos1) {
          if (pos < Prepos1) pos++;
          else if (pos > Prepos1) pos--;
          servo1.write(pos);
          delay(20);
        }

        while (pos2 != Prepos2) {
          if (pos2 < Prepos2) pos2++;
          else if (pos2 > Prepos2) pos2--;
          servo2.write(pos2);
          delay(20);
        }
        }
        break;

      case '0': //Preposicion Reset
        if (currentCommand == '0'){ // Reset
        Prepos1 = 90;
        Prepos2 = 90;
         
        while (pos != Prepos1) {
          if (pos < Prepos1) pos++;
          else if (pos > Prepos1) pos--;
          servo1.write(pos);
          delay(20);
        }

        while (pos2 != Prepos2) {
          if (pos2 < Prepos2) pos2++;
          else if (pos2 > Prepos2) pos2--;
          servo2.write(pos2);
          delay(20);
        }  
      }
        break;
    }
  }
}

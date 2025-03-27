#include <SoftwareSerial.h>
#include <Servo.h>

//Servos 
int posrotor = 90;
int posgarra = 180;
int pos = 90;
int pos2 = 90;
    int Prepos1;
    int Prepos2;
    int Preposgarra;
    int Preposrotor;

Servo servo1;
Servo servo2;
Servo servogarra;
Servo servorotor;

void setup() {
  
  Serial.begin(115200);

servogarra.attach(12);
  servo1.attach(9);
  servo2.attach(10);
  servorotor.attach(8);

  Serial.println("Conexion establecida");

}

void loop() {
  if (Serial.available() > 0){
    byte currentCommand = Serial.read();
    Serial.println(currentCommand);

    switch (currentCommand) {
      
      case '113': //q
        if (pos > 0) {
      pos--;  // Decrementar la posición
      servo1.write(pos); 
      delay (20);  // Controlar la velocidad del movimiento
    }
    break;

      case '97': //a
        if (pos < 180) {
        pos++;  
        servo1.write(pos);
        delay(20); 
        }
      break;

      case '119': //w
        if (pos2 > 0) {
      pos2--;  
      servo2.write(pos2); 
      delay (20); 
    }
    break;

      case '115': //s
        if (pos2 < 180) {
        pos2++;  
        servo2.write(pos2);
        delay(20); 
        }
      break;

    case '101': //e
        if (posgarra > 0) {
      posgarra--;  //derecha
      servogarra.write(posgarra); 
      delay (20); 
    }
    break;

      case '100': //d
        if (posgarra < 180) {
        posgarra++;  //izquierda
        servogarra.write(posgarra);
        delay(20); 
        }
      break;

      case '114': //r
        if (posrotor > 0) {
      posrotor--;  
      servorotor.write(posrotor); 
      delay (20); 
    }
    break;

      case '102': //f
        if (posrotor < 180) {
        posrotor++;  
        servorotor.write(posrotor);
        delay(20); 
        }
      break;
      case '122': //z
      Prepos1 = 90;
      Prepos2 = 90;
      Preposgarra = 90;
      Preposrotor = 180;


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

        while (posrotor != Preposrotor) {
          if (posrotor < Preposrotor) posrotor++;
          else if (posrotor > Preposrotor) posrotor--;
          servorotor.write(posrotor);
          delay(20);
        }

        while (posgarra != Preposgarra) {
          if (posgarra < Preposgarra) posgarra++;
          else if (posrotor > Preposgarra) posgarra--;
          servogarra.write(posgarra);
          delay(20);
        }
        break;


      case '120': //x
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
        break;
      default:
      break;
    }
  }

}

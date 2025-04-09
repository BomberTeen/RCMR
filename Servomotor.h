#ifndef Servomotor_h
#define Servomotor_h

#include <Servo.h>
#include <Arduino.h>

class Servomotor{
    public:
    Servo servo;
    unsigned long Actual;
    unsigned long Delay = 11500;
    
    unsigned long last;
    int pos;
    int pin;

    Servomotor (){
      pos = 90;
      last = 0;
      servo.write(pos);
    }  

  void set(int pin){
    servo.attach(pin);
    servo.write(pos);
  }

  void SetPosition(int Prepos){
      while(pos != Prepos){
        Actual = micros();
        if (Actual - last >= Delay){
          if (pos < Prepos) pos++;
          else if (pos > Prepos) pos--;

        }
      }
  }


  void aumentar(){
    Actual = micros();
    if (Actual - last >= Delay){
      last = Actual;
      if (pos < 180) pos++;
      servo.write(pos);
    }

  }

  void disminuir(){
    Actual = micros();
    if (Actual - last >= Delay){
      last = Actual;
      if(pos > 0) pos--;
      servo.write(pos);
    }

  }


void abrir(){
  servo.write(180);
}

void cerrar(){
  servo.write(0);
}
};
#endif
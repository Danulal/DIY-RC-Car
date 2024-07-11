#include <Arduino.h>
#include "../lib/Bluetooth/bluetooth.h"

static constexpr int PWM_STEER = 5;
static constexpr int PWM_ACCELL = 6;
char btVal;
char btSignal[7];

void setup() {
  pinMode(PWM_STEER, OUTPUT);
  pinMode(PWM_ACCELL, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  if(Serial.available()){
    readBT(btSignal);
  }

  if(btVal == '1'){
    analogWrite(PWM_STEER, 255);
  }
  else if(btVal == '2'){
    analogWrite(PWM_STEER, 0);
  }
}


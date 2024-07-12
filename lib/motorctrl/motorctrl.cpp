#include <Arduino.h>
#include "motorctrl.h"

int pwm;

void accel(int pin, int throttle, bool reversed) {
    pwm = map(throttle, 1000, 2000, 0, 255);
    analogWrite(pin, pwm);
}

void steer(int pin, int deflection);

bool reverse(int pin, int CH);
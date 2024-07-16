#include <Arduino.h>
#include "motorctrl.h"

int drive_pwm;
int steer_pwm;
static constexpr bool accel_reversed = false;       // set the corresponding bool to true if your motor is going the wrong direction
static constexpr bool steering_reversed = false;    // you can also just rewire the motors

static constexpr int min_pwm_steer = 0;             // minimum pwm value for steering
static constexpr int max_pwm_steer = 130;           // maximum pwm value for steering
static constexpr int min_pwm_drive = 60;             // minimum pwm value for driving
static constexpr int max_pwm_drive = 200;           // maximum pwm value for driving

unsigned long start_millis_motor = 0; // variables needed for the timer in drive_reverse()
unsigned long current_millis_motor = 0;
unsigned long millis_delay_motor; 
bool already_reversed = false;
bool currently_paused = false;
static constexpr long int max_reverse_time = 1720; // max time needed to stop motor for a reverse in ms


void accel(int pwm_pin, int reverse_pin, int throttle, bool reversed) { // control the accelerating motor
    if(accel_reversed) {
        reversed ^= 1;
    }
    drive_reverse(reverse_pin, pwm_pin, drive_pwm, reversed);

    if (throttle <= 1000) {
        drive_pwm = 0;
    } else {
        drive_pwm = map(throttle, 990, 2011, min_pwm_drive, max_pwm_drive);
    }
    if(currently_paused == false) {
        analogWrite(pwm_pin, drive_pwm);
    }
}

void steer(int pwm_pin, int reverse_pin, int deflection) { // control the steering motor
    if(deflection > 1550){
        steer_pwm = map(deflection, 1550, 2011, min_pwm_steer, max_pwm_steer);
        if(steering_reversed){
            digitalWrite(reverse_pin, 1);
        } else {
            digitalWrite(reverse_pin, 0);
        }
        analogWrite(pwm_pin, steer_pwm);
    } else if(deflection < 1450) {
        steer_pwm = map(deflection, 1450, 990, min_pwm_steer, max_pwm_steer);
        if(steering_reversed) {
            digitalWrite(reverse_pin, 0);
        } else {
            digitalWrite(reverse_pin, 1);
        }
        analogWrite(pwm_pin, steer_pwm);
    } else {
        steer_pwm = 0;
        analogWrite(pwm_pin, steer_pwm);
    }
}


void drive_reverse(int reverse_pin, int pwm_pin, int current_pwm, bool reversed) {
    if (current_millis_motor == 0){
        start_millis_motor = millis();
    }
    current_millis_motor = millis();
    millis_delay_motor = map(current_pwm, 0, 255, 0, max_reverse_time * 64); // *64 to adjust for the increased frequency of timer0

    // Serial.print("current_millis_motor: "); // timer debug
    // Serial.print(current_millis_motor);
    // Serial.print("\t");
    // Serial.print("start_millis_motor: ");
    // Serial.print(start_millis_motor);
    // Serial.print("\t");
    // Serial.print("actual millis: ");
    // Serial.print(millis());
    // Serial.print("\t");
    // Serial.print("millis_delay_motor: ");
    // Serial.print(millis_delay_motor);
    // Serial.print("\n");

        if(reversed && !already_reversed) {
            currently_paused = true;
            analogWrite(pwm_pin, 0);
            if(current_millis_motor - start_millis_motor >= millis_delay_motor) { // code that executes after calculated delay (from motor speed)
                digitalWrite(reverse_pin, 1);
                already_reversed = true;
                currently_paused = false;
                // Serial.println("Reversed1"); // timer debug
            }
        } else if(!reversed && already_reversed) {
            currently_paused = true;
            analogWrite(pwm_pin, 0);
            if(current_millis_motor - start_millis_motor >= millis_delay_motor) { // code that executes after calculated delay (from motor speed)
                digitalWrite(reverse_pin, 0);
                already_reversed = false;
                currently_paused = false;
                // Serial.println("Reversed2"); // timer debug
            }
        } else {
            start_millis_motor = current_millis_motor; 
            currently_paused = false; // unpauses the motor when the reverse switch is flicked on and off too quickly
        }    
}

void stop(int steer_pwm_pin, int drive_pwm_pin) {
    steer(steer_pwm_pin, 0, 1500);
    accel(drive_pwm_pin, 0, 0, false);
}
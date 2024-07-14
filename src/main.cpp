#include <Arduino.h>

#include "motorctrl.h"
#include "elrs.h"

static constexpr int PWM_STEER = 5; // define arduino pins
static constexpr int PWM_ACCEL = 6;
static constexpr int STEER_REVERSE = 7;
static constexpr int ACCEL_REVERSE = 10;  
// static constexpr int LED = ?;

static constexpr int CH_STEER = 1; // define elrs receiver channels       
static constexpr int CH_THROTTLE = 3;
static constexpr int CH_REVERSE = 10;
static constexpr int CH_ARM = 6;
static constexpr int CH_BRAKE = 9;
static constexpr int CH_LED = 8;

int throttle;
int steering_deflection;
bool armed;
bool reversed;

void setup() {
  pinMode(PWM_STEER, OUTPUT); // set output pins
  pinMode(PWM_ACCEL, OUTPUT);
  pinMode(STEER_REVERSE, OUTPUT);
  pinMode(ACCEL_REVERSE, OUTPUT);
  init_elrs(115200); // initialise crsf connection with baudrate 115200

//---------------------------------------------- Set PWM frequency for D5 & D6 -------------------------------
//NOTE: Changing this timer 0 affects millis() and delay!
TCCR0B = TCCR0B & B11111000 | B00000001;    // set timer 0 divisor to     1 for PWM frequency of 62500.00 Hz    with this the motor doesn't whine
// TCCR0B = TCCR0B & B11111000 | B00000010;    // set timer 0 divisor to     8 for PWM frequency of  7812.50 Hz
// TCCR0B = TCCR0B & B11111000 | B00000011;    // set timer 0 divisor to    64 for PWM frequency of   976.56 Hz (The DEFAULT)
// TCCR0B = TCCR0B & B11111000 | B00000100;    // set timer 0 divisor to   256 for PWM frequency of   244.14 Hz
// TCCR0B = TCCR0B & B11111000 | B00000101;    // set timer 0 divisor to  1024 for PWM frequency of    61.04 Hz
}

void loop() {
  update_elrs(); // Must call update_elrs() in loop() to process data
  throttle = readCH(CH_THROTTLE);
  steering_deflection = readCH(CH_STEER);
  armed = elrs_2way_switch(CH_ARM);
  reversed = elrs_2way_switch(CH_REVERSE);

  // printChannels();

  steer(PWM_STEER, STEER_REVERSE, steering_deflection);
  accel(PWM_ACCEL, ACCEL_REVERSE, throttle, reversed);
}
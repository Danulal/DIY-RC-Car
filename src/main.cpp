#include <Arduino.h>

#include "motorctrl.h"
#include "elrs.h"

static constexpr int PWM_STEER = 5; // define motor pins for steering and accelerating
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
int steering;

void setup()
{
  pinMode(PWM_STEER, OUTPUT);
  pinMode(PWM_ACCEL, OUTPUT);
  init_elrs(115200);
}

// Use crsf.getChannel(x) to get us channel values (1-16).

void loop()
{
  // Must call update_elrs() in loop() to process data
  update_elrs();
  throttle = readCH(CH_THROTTLE);
  steering = readCH(CH_STEER);
  printChannels();
  // accel(PWM_ACCEL, throttle);
}

#ifndef MOTORCTRL
#define MOTORCTRL
void accel(int pwm_pin, int reverse_pin, int throttle, bool reversed, bool quickreverse);
void steer(int pwm_pin, int reverse_pin, int deflection);
void drive_reverse(int reverse_pin, int pwm_pin, int current_pwm, bool reversed, bool quickreverse);
void stop(int steer_pwm_pin, int drive_pwm_pin);
void quick_reverse(int pwm_pin, int reverse_pin, bool reversed);
#endif // MOTORCTRL
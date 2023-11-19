#include "movement.h"

#include "lcd/lcd.h"
#include "main.h"
#include "math.h"

const int16_t Kp = 9;
const int16_t Ki = 0;
const int16_t Kd = 50;

void move(Motor frontL_motor, Motor frontR_motor, Motor back_motor, int16_t x, int16_t y, int16_t direction, int16_t speed,
          int16_t *last_error_frontL, int16_t *last_error_frontR, int16_t *last_error_back) {
    double frontL_rpm = (x / 100.0) * 0.5 + (y / 100.0) * sqrt(3) / 2.0 + (direction / 100.0);
    double frontR_rpm = (x / 100.0) * 0.5 - (y / 100.0) * sqrt(3) / 2.0 + (direction / 100.0);
    double back_rpm = -(x / 100.0) + (direction / 100.0);

    frontL_rpm *= speed;
    frontR_rpm *= speed;
    back_rpm *= speed;

    set_motor_speed(frontL_motor, frontL_rpm, Kp, Ki, Kd, last_error_frontL);
    set_motor_speed(frontR_motor, frontR_rpm, Kp, Ki, Kd, last_error_frontR);
    set_motor_speed(back_motor, back_rpm, Kp, Ki, Kd, last_error_back);

    tft_prints(0, 5, "FL: %0.3f ", frontL_rpm);
    tft_prints(0, 6, "FR: %0.3f ", frontR_rpm);
    tft_prints(0, 7, "B:  %0.3f ", back_rpm);
}

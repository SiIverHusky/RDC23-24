#include "movement.h"

#include "lcd/lcd.h"
#include "main.h"
#include "math.h"

const int16_t Kp = 7;
const int16_t Ki = 0;
const int16_t Kd = 0;

void move(Motor frontL_motor, Motor frontR_motor, Motor back_motor, double x, double y, double direction,
          int16_t *last_error) {
    double radius = 0.0; // center of robot to wheel
    double frontL_rpm = x * 0.5 + y * sqrt(3) / 2.0 - direction;
    double frontR_rpm = x * 0.5 + y * sqrt(3) / 2.0 - direction;
    double back_rpm = x - radius * direction;

    if (frontL_rpm > 1.0 || frontR_rpm > 1.0 || back_rpm > 1.0) {
        if (frontL_rpm >= frontR_rpm && frontL_rpm >= back_rpm) {
            frontL_rpm = frontL_rpm / frontL_rpm;
            frontR_rpm = frontR_rpm / frontL_rpm;
            back_rpm = back_rpm / frontL_rpm;
        } else if (frontR_rpm >= frontL_rpm && frontR_rpm >= back_rpm) {
            frontL_rpm = frontL_rpm / frontR_rpm;
            frontR_rpm = frontR_rpm / frontR_rpm;
            back_rpm = back_rpm / frontR_rpm;
        } else if (back_rpm >= frontL_rpm && back_rpm >= frontR_rpm) {
            frontL_rpm = frontL_rpm / back_rpm;
            frontR_rpm = frontR_rpm / back_rpm;
            back_rpm = back_rpm / back_rpm;
        }
    }

    frontL_rpm *= 500.0;
    frontR_rpm *= 500.0;
    back_rpm *= 500.0;

    set_motor_speed(frontL_motor, frontL_rpm, Kp, Kd, Ki, last_error);
    set_motor_speed(frontR_motor, frontR_rpm, Kp, Kd, Ki, last_error);
    set_motor_speed(back_motor, back_rpm, Kp, Kd, Ki, last_error);
}

void test_movement(Motor frontL_motor, Motor frontR_motor, Motor back_motor, uint8_t move_value, int16_t *last_error) {
    switch (move_value) {
        case 0: // stop
            move(frontL_motor, frontR_motor, back_motor, 0.0, 0.0, 0.0, last_error);
            tft_prints(0, 0, "STOP");
            break;
        case 1: // forwards
            move(frontL_motor, frontR_motor, back_motor, 0.0, 0.5, 0.0, last_error);
            tft_prints(0, 0, "FORWARDS");
            break;
        case 2: // forwards right
            move(frontL_motor, frontR_motor, back_motor, -0.5, 0.5, 0.0, last_error);
            tft_prints(0, 0, "FORWARDS R");
            break;
        case 3: // right
            move(frontL_motor, frontR_motor, back_motor, -0.5, 0.0, 0.0, last_error);
            tft_prints(0, 0, "RIGHT");
            break;
        case 4: // backwards right
            move(frontL_motor, frontR_motor, back_motor, -0.5, -0.5, 0.0, last_error);
            tft_prints(0, 0, "BACKWARDS R");
            break;
        case 5: // backwards
            move(frontL_motor, frontR_motor, back_motor, 0.0, -0.5, 0.0, last_error);
            tft_prints(0, 0, "BACKWARDS");
            break;
        case 6: // backwards left
            move(frontL_motor, frontR_motor, back_motor, -0.5, -0.5, 0.0, last_error);
            tft_prints(0, 0, "BACKWARDS L");
            break;
        case 7: // left
            move(frontL_motor, frontR_motor, back_motor, -0.5, 0.0, 0.0, last_error);
            tft_prints(0, 0, "LEFT");
            break;
        case 8: // forwards left
            move(frontL_motor, frontR_motor, back_motor, -0.5, 0.5, 0.0, last_error);
            tft_prints(0, 0, "FORWARDS L");
            break;
        case 9: // rotate left
            move(frontL_motor, frontR_motor, back_motor, 0.0, 0.0, 0.5, last_error);
            tft_prints(0, 0, "ROTATE L");
            break;
        case 10: // rotate right
            move(frontL_motor, frontR_motor, back_motor, 0.0, 0.0, -0.5, last_error);
            tft_prints(0, 0, "ROTATE R");
            break;
        default: break;
    }
}


#include "movement.h"

#include "lcd/lcd.h"
#include "main.h"
#include "math.h"

const int16_t Kp = 9;
const int16_t Ki = 0;
const int16_t Kd = 50;

void move(Motor frontL_motor, Motor frontR_motor, Motor back_motor, double x, double y, double direction,
          int16_t *last_error_frontL, int16_t *last_error_frontR, int16_t *last_error_back) {
    double radius = 0.0; // center of robot to wheel
    double frontL_rpm = x * 0.5 + y * sqrt(3) / 2.0 + direction;
    double frontR_rpm = x * 0.5 - y * sqrt(3) / 2.0 + direction;
    double back_rpm = x + direction;

    if (fabs(frontL_rpm) > 1.0 || fabs(frontR_rpm) > 1.0 || fabs(back_rpm) > 1.0) {
        if (fabs(frontL_rpm) >= fabs(frontR_rpm) && fabs(frontL_rpm) >= fabs(back_rpm)) {
            frontL_rpm /= fabs(frontL_rpm);
            frontR_rpm /= fabs(frontL_rpm);
            back_rpm /= fabs(frontL_rpm);
        } else if (fabs(frontR_rpm) >= fabs(frontL_rpm) && fabs(frontR_rpm) >= fabs(back_rpm)) {
            frontL_rpm /= fabs(frontR_rpm);
            frontR_rpm /= fabs(frontR_rpm);
            back_rpm /= fabs(frontR_rpm);
        } else if (fabs(back_rpm) >= fabs(frontL_rpm) && fabs(back_rpm) >= fabs(frontR_rpm)) {
            frontL_rpm /= fabs(back_rpm);
            frontR_rpm /= fabs(back_rpm);
            back_rpm /= fabs(back_rpm);
        }
    }

    frontL_rpm *= 500.0;
    frontR_rpm *= 500.0;
    back_rpm *= 500.0;

    set_motor_speed(frontL_motor, frontL_rpm, Kp, Ki, Kd, last_error_frontL);
    set_motor_speed(frontR_motor, frontR_rpm, Kp, Ki, Kd, last_error_frontR);
    set_motor_speed(back_motor, back_rpm, Kp, Ki, Kd, last_error_back);

    tft_prints(0, 0, "FL: %0.3f", frontL_rpm);
    tft_prints(0, 1, "FR: %0.3f", frontR_rpm);
    tft_prints(0, 2, "B:  %0.3f", back_rpm);

}

void test_movement(Motor frontL_motor, Motor frontR_motor, Motor back_motor, uint8_t move_value, int16_t *last_error_frontL, int16_t *last_error_frontR, int16_t *last_error_back) {
    switch (move_value) {
        case 0: // stop
            move(frontL_motor, frontR_motor, back_motor, 0.0, 0.0, 0.0, last_error_frontL, last_error_frontR, last_error_back);
            tft_prints(0, 0, "STOP");
            break;
        case 1: // forwards
            move(frontL_motor, frontR_motor, back_motor, 0.0, 0.5, 0.0, last_error_frontL, last_error_frontR, last_error_back);
            tft_prints(0, 0, "FORWARDS");
            break;
        case 2: // forwards right
            move(frontL_motor, frontR_motor, back_motor, -0.5, 0.5, 0.0, last_error_frontL, last_error_frontR, last_error_back);
            tft_prints(0, 0, "FORWARDS R");
            break;
        case 3: // right
            move(frontL_motor, frontR_motor, back_motor, -0.5, 0.0, 0.0, last_error_frontL, last_error_frontR, last_error_back);
            tft_prints(0, 0, "RIGHT");
            break;
        case 4: // backwards right
            move(frontL_motor, frontR_motor, back_motor, -0.5, -0.5, 0.0, last_error_frontL, last_error_frontR, last_error_back);
            tft_prints(0, 0, "BACKWARDS R");
            break;
        case 5: // backwards
            move(frontL_motor, frontR_motor, back_motor, 0.0, -0.5, 0.0, last_error_frontL, last_error_frontR, last_error_back);
            tft_prints(0, 0, "BACKWARDS");
            break;
        case 6: // backwards left
            move(frontL_motor, frontR_motor, back_motor, -0.5, -0.5, 0.0, last_error_frontL, last_error_frontR, last_error_back);
            tft_prints(0, 0, "BACKWARDS L");
            break;
        case 7: // left
            move(frontL_motor, frontR_motor, back_motor, -0.5, 0.0, 0.0, last_error_frontL, last_error_frontR, last_error_back);
            tft_prints(0, 0, "LEFT");
            break;
        case 8: // forwards left
            move(frontL_motor, frontR_motor, back_motor, -0.5, 0.5, 0.0, last_error_frontL, last_error_frontR, last_error_back);
            tft_prints(0, 0, "FORWARDS L");
            break;
        case 9: // rotate left
            move(frontL_motor, frontR_motor, back_motor, 0.0, 0.0, 0.5, last_error_frontL, last_error_frontR, last_error_back);
            tft_prints(0, 0, "ROTATE L");
            break;
        case 10: // rotate right
            move(frontL_motor, frontR_motor, back_motor, 0.0, 0.0, -0.5, last_error_frontL, last_error_frontR, last_error_back);
            tft_prints(0, 0, "ROTATE R");
            break;
        default: break;
    }
}

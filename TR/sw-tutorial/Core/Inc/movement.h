#ifndef __MOVEMENT_H__
#define __MOVEMENT_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "can.h"
#include "main.h"
#include "math.h"

void test_movement(Motor frontL_motor, Motor frontR_motor, Motor back_motor, uint8_t move, int16_t *last_error);

void move(Motor frontL_motor, Motor frontR_motor, Motor back_motor, double x, double y, double direction,
          int16_t *last_error);

#ifdef __cplusplus
}
#endif

#endif /* __MOVEMENT_H__*/
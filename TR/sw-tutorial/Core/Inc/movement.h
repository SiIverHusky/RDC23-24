#ifndef __MOVEMENT_H__
#define __MOVEMENT_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "can.h"
#include "main.h"
#include "math.h"

void move(Motor frontL_motor, Motor frontR_motor, Motor back_motor, int16_t x, int16_t y, int16_t direction,
          int16_t speed, int16_t *last_error_frontL, int16_t *last_error_frontR, int16_t *last_error_back);

#ifdef __cplusplus
}
#endif

#endif /* __MOVEMENT_H__*/

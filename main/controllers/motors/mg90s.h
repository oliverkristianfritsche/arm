#ifndef MG90S_H
#define MG90S_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/mcpwm.h"
#include "soc/mcpwm_periph.h"
#include <math.h>
#include <unistd.h>

#ifdef __cplusplus
extern "C" {
#endif

void mcpwm_initialize_all(void);

void set_motors(float *angles);

void set_servo_angle(mcpwm_unit_t unit, mcpwm_timer_t timer, float angle);



#ifdef __cplusplus
}
#endif

#endif // MG90S_H

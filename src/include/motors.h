#ifndef __MOTORS_H
#define __MOTORS_H

#include "config.h"
#include "delay.h"
#include "utils.h"

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>

bool is_esc_inited(void);
void init_esc(void);
void set_motors_speed(float velI, float velD);
void set_fan_speed(uint8_t vel);
void set_fans_speed(int8_t velI, int8_t velD);

#endif
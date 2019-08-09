#ifndef __MOTORS_H
#define __MOTORS_H

#include <stdint.h>

#include <config.h>
#include <delay.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>

bool is_esc_inited();
void init_esc();

#endif
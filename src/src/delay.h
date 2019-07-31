#ifndef __DELAY_H
#define __DELAY_H

#include <stdint.h>

#include <libopencm3/cm3/dwt.h>

#include "setup.h"

void delay(uint32_t ms);
void delay_us(uint32_t us);
void clock_tick();
uint32_t get_clock_ticks(void);
uint32_t read_cycle_counter();

#endif
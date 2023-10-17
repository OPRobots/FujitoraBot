#ifndef __DELAY_H
#define __DELAY_H

#include "setup.h"

#include <libopencm3/cm3/dwt.h>
#include <stdint.h>

void delay(uint32_t ms);
void delay_us(uint32_t us);
void clock_tick(void);
uint32_t get_clock_ticks(void);
uint32_t read_cycle_counter(void);

#endif
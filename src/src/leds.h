#ifndef __SETUP_H
#define __SETUP_H

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>
#include <delay.h>
#include <config.h>

void set_status_led(bool state);
void toggle_status_led();
void set_RGB_color(uint32_t r, uint32_t g, uint32_t b);
void set_RGB_rainbow();
void set_neon_fade(uint32_t n);
void set_neon_heartbeat();

#endif
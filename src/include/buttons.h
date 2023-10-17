#ifndef __BUTTONS_H
#define __BUTTONS_H

#include "config.h"
#include "delay.h"
#include <math.h>

#include <libopencm3/stm32/gpio.h>
#include <stdio.h>

bool get_start_btn(void);
bool get_menu_up_btn(void);
bool get_menu_down_btn(void);
bool get_menu_mode_btn(void);
bool get_swtich_1(void);
bool get_swtich_2(void);
bool get_swtich_3(void);
uint8_t get_switch_decimal(void);

#endif
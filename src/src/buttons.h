#ifndef __BUTTONS_H
#define __BUTTONS_H

#include <config.h>
#include <delay.h>
#include <math.h>
#include <stdio.h>
#include <libopencm3/stm32/gpio.h>

bool get_start_btn();
bool get_menu_up_btn();
bool get_menu_down_btn();
bool get_menu_mode_btn();
bool get_swtich_1();
bool get_swtich_2();
bool get_swtich_3();
uint8_t get_switch_decimal();

#endif
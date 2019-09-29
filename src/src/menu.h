#ifndef __MENU_H
#define __MENU_H

#include <config.h>
#include <debug.h>
#include <buttons.h>
#include <leds.h>

void check_menu_button();
uint8_t get_base_speed();
uint8_t get_base_fan_speed();
bool in_debug_mode();
void reset_menu_mode();

#endif
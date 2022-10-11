#ifndef __MENU_H
#define __MENU_H

#include <config.h>
#include <debug.h>
#include <buttons.h>
#include <leds.h>
#include <sensors.h>

void check_menu_button();
uint8_t get_base_speed();
float get_base_ms_speed();
float get_robotracer_straight_ms_speed();
float get_base_acceleration_mss();
float get_base_deceleration_mss();
float get_base_turn_acceleration_mss();
uint8_t get_base_fan_speed();
bool in_debug_mode();
void reset_menu_mode();

#endif
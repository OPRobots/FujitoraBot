#ifndef __MENU_H
#define __MENU_H

#include "buttons.h"
#include "config.h"
#include "debug.h"
#include "leds.h"
#include "sensors.h"

#define MODE_NOTHING 0
#define MODE_SPEED 1
#define MODE_FANS 2
#define MODE_DEBUG 3
#define NUM_MODOS_RACE 3
#define NUM_MODOS_DEBUG 4

#define NUM_VALORES 9
#define NUM_VALORES_TIPO_MORRO 2

void check_menu_button(void);
uint8_t get_base_speed(void);
float get_base_ms_speed(void);
float get_robotracer_straight_ms_speed(void);
float get_base_acceleration_mss(void);
float get_base_deceleration_mss(void);
float get_base_turn_acceleration_mss(void);
uint8_t get_base_fan_speed(void);
bool in_debug_mode(void);
void reset_menu_mode(void);

#endif
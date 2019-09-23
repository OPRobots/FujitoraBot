#ifndef __DEBUG_H
#define __DEBUG_H

#include <stdint.h>

#include <buttons.h>
#include <config.h>
#include <delay.h>
#include <motors.h>
#include <sensors.h>
#include <encoders.h>
#include <usart.h>


void debug_sensors_raw();
void debug_sensors_calibrated();
void debug_all_leds();
void debug_digital_io();
void debug_line_position();
void debug_motors();
void debug_from_switch();

#endif
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

#define DEBUG_TYPE_SENSORS_RAW 0
#define DEBUG_TYPE_SENSORS_CALIBRATED 1
#define DEBUG_TYPE_LINE_POSITION 2
#define DEBUG_TYPE_MOTORS 3
#define DEBUG_TYPE_ENCODERS 4
#define DEBUG_TYPE_DIGITAL_IO 5
#define DEBUG_TYPE_LEDS_PARTY 8
#define DEBUG_TYPE_FANS_DEMO 9



void debug_from_switch();
void debug_from_config(uint8_t type);

#endif
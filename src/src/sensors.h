#ifndef __SENSORS_H
#define __SENSORS_H

#include <libopencm3/stm32/adc.h>
#include <stdint.h>
#include <delay.h>
#include <leds.h>
#include <motors.h>
#include <buttons.h>
#include <config.h>

void calibrate_sensors();
uint8_t *get_sensors();
uint8_t get_sensors_num();
uint8_t get_sensors_line_num();
volatile uint16_t *get_sensors_raw();
uint16_t get_sensor_raw(uint8_t pos);
uint16_t get_sensor_calibrated(uint8_t pos);
int32_t get_sensor_line_position();
void calc_sensor_line_position();
void check_side_marks();
bool is_left_mark();
bool is_right_mark();

#endif
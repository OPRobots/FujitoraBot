#ifndef __SENSORS_H
#define __SENSORS_H

#include "buttons.h"
#include "config.h"
#include "delay.h"
#include "encoders.h"
#include "leds.h"
#include "motors.h"
#include "eeprom.h"

#include <libopencm3/stm32/adc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <stdint.h>

#define NUM_SENSORES_LINEFOLLOWER 16
#define NUM_SENSORES_ROBOTRACER 12
#define NUM_SENSORES_ROBOTRACER_MARCAS 4
#define NUM_SENSORES_MAX 16

void print_sensors_calibrations(void);
void calibrate_sensors(void);
uint8_t *get_sensors(void);
uint8_t get_sensors_num(void);
uint8_t get_sensors_line_num(void);
volatile uint16_t *get_sensors_raw(void);
uint16_t get_sensor_raw(uint8_t pos);
uint16_t get_sensor_calibrated(uint8_t pos);
int32_t get_sensor_line_position(void);
void calc_sensor_line_position(void);
void check_side_marks(void);
bool is_left_mark(void);
bool is_right_mark(void);

uint32_t get_millis_emergency_stop(void);
void reset_millis_emergency_stop(void);
void emergency_stop(void);

#endif
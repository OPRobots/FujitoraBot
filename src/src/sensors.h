#ifndef __SENSORS_H
#define __SENSORS_H

#include <buttons.h>
#include <config.h>
#include <delay.h>
#include <encoders.h>
#include <leds.h>
#include <libopencm3/stm32/adc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <motors.h>
#include <stdint.h>

#define TIPO_MORRO_LARGO 1
#define TIPO_MORRO_CORTO 2

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

uint8_t get_tipo_morro();
void toggle_tipo_morro();
void activar_morro_largo();
void activar_morro_corto();

#endif
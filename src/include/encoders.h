#ifndef __ENCODERS_H
#define __ENCODERS_H

#include "config.h"
#include "setup.h"

#include <libopencm3/stm32/timer.h>
#include <stdint.h>

float get_wheels_separation(void);
void set_wheels_separation(float value);
int32_t get_encoder_left_diff_ticks(void);
int32_t get_encoder_right_diff_ticks(void);
int32_t get_encoder_left_total_ticks(void);
int32_t get_encoder_right_total_ticks(void);
int32_t get_encoder_left_micrometers(void);
int32_t get_encoder_right_micrometers(void);
int32_t get_encoder_average_micrometers(void);
float get_encoder_left_speed(void);
float get_encoder_right_speed(void);
float get_encoder_avg_speed(void);
float get_encoder_angular_speed(void);

float get_encoder_avg_micrometers(void);
float get_encoder_curernt_angle(void);
int32_t get_encoder_x_position(void);
int32_t get_encoder_y_position(void);

int32_t max_likelihood_counter_diff(uint16_t now, uint16_t before);
void update_encoder_readings(void);

#endif

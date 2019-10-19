#ifndef __CONTROL_H
#define __CONTROL_H

#include <config.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>
#include <math.h>
#include <sensors.h>
#include <stdio.h>
#include <utils.h>


bool is_competicion_iniciada();
void set_competicion_iniciada(bool state);

void pid_speed_timer_custom_isr();
float calc_pid_correction(int32_t posicion);
void speed_timer_custom_isr();
float get_speed_correction();
void set_ideal_motors_speed(int32_t v);
void set_ideal_fan_speed(int32_t v);
void resume_pid_speed_timer();
void pause_pid_speed_timer();

#endif
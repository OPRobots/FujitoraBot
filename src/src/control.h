#ifndef __CONTROL_H
#define __CONTROL_H

#include <config.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>
#include <math.h>
#include <sensors.h>
#include <encoders.h>
#include <usart.h>
#include <stdio.h>
#include <utils.h>
#include <delay.h>
#include <leds.h>
#include <robotracer.h>


bool is_competicion_iniciada();
void set_competicion_iniciada(bool state);

void pid_speed_timer_custom_isr();
float calc_pid_correction(int32_t posicion);
float calc_ms_pid_correction(float velocidadActualMs);
void speed_timer_custom_isr();
float get_speed_correction();
void set_ideal_motors_speed(int32_t v);
void set_ideal_motors_ms_speed(float ms);
void set_acceleration_mss(float mss);
void set_deceleration_mss(float mss);
void set_ideal_fan_speed(int32_t v);
void resume_pid_speed_timer();
void pause_pid_speed_timer();
void debug_accel();

#endif
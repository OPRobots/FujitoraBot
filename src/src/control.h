#ifndef __CONTROL_H
#define __CONTROL_H

#include <config.h>
#include <math.h>
#include <stdio.h>
#include <libopencm3/stm32/rcc.h>

void pid_timer_custom_isr();
void speed_timer_custom_isr();

void resume_pid_timer();
void pause_pid_timer();
void resume_speed_timer();
void pause_speed_timer();

#endif
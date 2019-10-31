#ifndef __CONFIG_H
#define __CONFIG_H
#include <buttons.h>
#include <stdio.h>


#define SYSCLK_FREQUENCY_HZ 168000000
#define SYSTICK_FREQUENCY_HZ 1000
#define MICROMETERS_PER_METER 1000000
#define MICROSECONDS_PER_SECOND 1000000
#define PI 3.1415

#define MS_CALIBRACION_LINEA 2000
#define LECTURA_MAXIMO_SENSORES_LINEA 4096
#define LECTURA_MINIMO_SENSORES_LINEA 0

#define LEDS_MAX_PWM 1024
#define MOTORES_MAX_PWM 1024

#define KP 0.03
#define KI 0
#define KD 2.5

#define KP_MS 10.0
#define KI_MS 1.2
#define KD_MS 20.0

#define MILLIS_INICIO 2000
#define TIEMPO_SIN_PISTA 150

#define MICROMETERS_PER_TICK 78.3699
#define WHEELS_SEPARATION 0.1169

#define MIN_SPEED_PERCENT 15
#define MAX_ACCEL_PERCENT 45.0
#define MAX_ACCEL_MS2 12.0

#define CONFIG_RUN_RACE 1
#define CONFIG_RUN_DEBUG 0

#define CONFIG_SPEED_MS 1
#define CONFIG_SPEED_PWM 0

#define CONFIG_TRACK_ROBOTRACER 1
#define CONFIG_TRACK_LINEFOLLOWER 0

void set_all_configs(void);
uint16_t get_config_run(void);
uint16_t get_config_speed(void);
uint16_t get_config_track(void);

uint16_t get_offtrack_time(void);

#endif
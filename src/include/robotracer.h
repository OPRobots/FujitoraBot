#ifndef __ROBOTRACER_H
#define __ROBOTRACER_H

#include "config.h"
#include "encoders.h"
#include "math.h"
#include "motors.h"
#include "usart.h"

enum SectorType { STRAIGHT,
                  TURN,
                  R10 };

// void check_sector_radius(void);
void robotracer_loop_flow(void);
void robotracer_restart(void);
void robotracer_set_turn_speed(float ms);
void robotracer_set_straight_speed(float ms);
void robotracer_set_acceleration_mss(float mss);
void robotracer_set_deceleration_mss(float mss);
void robotracer_set_turn_acceleration_mss(float mss);

#endif
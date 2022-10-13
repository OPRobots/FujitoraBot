#ifndef __ROBOTRACER_H
#define __ROBOTRACER_H

#include <encoders.h>
#include <usart.h>
#include <math.h>
#include <motors.h>
#include <config.h>

enum SectorType { STRAIGHT,
                  TURN,
                  R10 };

// void check_sector_radius();
void robotracer_loop_flow();
void robotracer_restart();
void robotracer_set_turn_speed(float ms);
void robotracer_set_straight_speed(float ms);
void robotracer_set_acceleration_mss(float mss);
void robotracer_set_deceleration_mss(float mss);
void robotracer_set_turn_acceleration_mss(float mss);

#endif
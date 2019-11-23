#ifndef __ROBOTRACER_H
#define __ROBOTRACER_H

#include <encoders.h>
#include <usart.h>
#include <math.h>
#include <vector.h>

enum SectorType { STRAIGHT,
                  TURN,
                  R10 };

// void check_sector_radius();
void check_next_sector_radius();
void robotracer_left_mark();
void robotracer_right_mark();
bool robotracer_can_stop();
void robotracer_check_sector_ends_before_mark();

#endif
#ifndef __ROBOTRACER_H
#define __ROBOTRACER_H

#include <encoders.h>
#include <usart.h>
#include <math.h>
#include <vector.h>

enum SectorType { STRAIGHT,
                  TURN,
                  R10 };

void check_sector_radius();
void map_left_mark();
void map_right_mark();

#endif
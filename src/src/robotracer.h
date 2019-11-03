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
void left_mark();
void right_mark();

#endif
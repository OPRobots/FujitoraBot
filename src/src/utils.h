#ifndef __UTILS_H
#define __UTILS_H

#include <stdint.h>

float map(float x, float in_min, float in_max, float out_min, float out_max);
int32_t constrain(int32_t x, int32_t min, int32_t max);

#endif
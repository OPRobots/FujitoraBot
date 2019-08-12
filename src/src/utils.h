#ifndef __UTILS_H
#define __UTILS_H

#include <stdint.h>

int32_t map(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max);
int32_t constrain(int32_t x, int32_t min, int32_t max);

#endif
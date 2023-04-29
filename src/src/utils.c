#include "utils.h"

float map(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float constrain(float x, float min, float max) {
  if (x > max) {
    return max;
  } else if (x < min) {
    return min;
  } else {
    return x;
  }
}

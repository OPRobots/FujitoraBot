#include <motors.h>

static bool escInited = false;
static uint32_t escInitMillis = 0;

bool is_esc_inited(){
  return escInited;
}

void init_esc() {
  if (!escInited) {
    if (escInitMillis == 0) {
      escInitMillis = get_clock_ticks();

      timer_enable_oc_output(TIM8, TIM_OC1);
      timer_enable_oc_output(TIM8, TIM_OC2);
      timer_enable_oc_output(TIM8, TIM_OC3);
      timer_enable_oc_output(TIM8, TIM_OC4);
    }
    if (get_clock_ticks() - escInitMillis < 5000) {
      timer_set_oc_value(TIM8, TIM_OC1, 768);
      timer_set_oc_value(TIM8, TIM_OC2, 768);
      timer_set_oc_value(TIM8, TIM_OC3, 768);
      timer_set_oc_value(TIM8, TIM_OC4, 768);
    } else {
      escInited = true;
    }
  }
}
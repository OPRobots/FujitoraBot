
#include <debug.h>
#include <delay.h>
#include <leds.h>
#include <motors.h>
#include <sensors.h>
#include <setup.h>
#include <usart.h>


void sys_tick_handler(void) {
  clock_tick();
  if (!is_esc_inited()) {
    init_esc();
  }
  calc_sensor_line_position();
}

int main(void) {

  setup();
  // calibrate_sensors();

  while (1) {
    // debug_line_position();

    // timer_set_oc_value(TIM8, TIM_OC1, 850);
    // timer_set_oc_value(TIM8, TIM_OC2, 850); // derecho
    // timer_set_oc_value(TIM8, TIM_OC3, 850);
    // timer_set_oc_value(TIM8, TIM_OC4, 850); // izquierdo
  }
}
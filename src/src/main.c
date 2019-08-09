
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

  bool tested = false;

  while (1) {
    // debug_line_position();
    if (is_esc_inited() && !tested) {
      set_motors_speed(30, 30);
      delay(1000);
      set_motors_speed(0, 0);
      tested = true;
    }
    // if (is_esc_inited() && !tested) {
    //   timer_set_oc_value(TIM8, TIM_OC1, 850);
    //   timer_set_oc_value(TIM8, TIM_OC2, 850); // derecho
    //   timer_set_oc_value(TIM8, TIM_OC3, 850);
    //   timer_set_oc_value(TIM8, TIM_OC4, 850); // izquierdo
    //   delay(2000);
    //   timer_set_oc_value(TIM8, TIM_OC1, 768);
    //   timer_set_oc_value(TIM8, TIM_OC2, 768); // derecho
    //   timer_set_oc_value(TIM8, TIM_OC3, 768);
    //   timer_set_oc_value(TIM8, TIM_OC4, 768); // izquierdo
    //   tested = true;
    // }
  }
}
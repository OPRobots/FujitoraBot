
#include <debug.h>
#include <delay.h>
#include <leds.h>
#include <sensors.h>
#include <setup.h>
#include <usart.h>

void sys_tick_handler(void) {
  clock_tick();
  calc_sensor_line_position();
}

int main(void) {

  setup();
  // calibrate_sensors();

  timer_enable_oc_output(TIM8, TIM_OC1);
  timer_enable_oc_output(TIM8, TIM_OC2);
  timer_enable_oc_output(TIM8, TIM_OC3);
  timer_enable_oc_output(TIM8, TIM_OC4);
  // timer_set_oc_value(TIM8, TIM_OC1, 1024);
  // timer_set_oc_value(TIM8, TIM_OC2, 1024);
  // timer_set_oc_value(TIM8, TIM_OC3, 1024);
  // timer_set_oc_value(TIM8, TIM_OC4, 1024);
  // delay(5000);
  timer_set_oc_value(TIM8, TIM_OC1, 768);
  timer_set_oc_value(TIM8, TIM_OC2, 768);
  timer_set_oc_value(TIM8, TIM_OC3, 768);
  timer_set_oc_value(TIM8, TIM_OC4, 768);
  delay(5000);

  while (1) {
    // debug_line_position();
    
      // timer_set_oc_value(TIM8, TIM_OC1, 850);
      // timer_set_oc_value(TIM8, TIM_OC2, 850); // derecho
      // timer_set_oc_value(TIM8, TIM_OC3, 850); 
      // timer_set_oc_value(TIM8, TIM_OC4, 850); // izquierdo
  }
}
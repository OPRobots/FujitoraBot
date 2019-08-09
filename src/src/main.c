
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
    debug_from_switch();
  }
}

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
  calibrate_sensors();

  while (1) {
    debug_line_position();
  }
}
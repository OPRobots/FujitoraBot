
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

  // Espera para calibrar sensores
  while (!get_start_btn()) {
    set_neon_heartbeat();
  }
  while (get_start_btn()) {
    set_neon_heartbeat();
  }
  set_neon_fade(0);
  delay(1000);
  calibrate_sensors();

  while (1) {
    debug_from_switch();
  }
}
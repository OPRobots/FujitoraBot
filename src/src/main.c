
#include <config.h>
#include <control.h>
#include <debug.h>
#include <delay.h>
#include <leds.h>
#include <menu.h>
#include <motors.h>
#include <sensors.h>
#include <setup.h>
#include <usart.h>

void sys_tick_handler(void) {
  clock_tick();
  if (!is_esc_inited()) {
    init_esc();
  }
}


int main(void) {
  setup();
  calibrate_sensors();

  while (1) {
    // printf("%d\n", get_toggle_ticks());
    // delay(25);
    // printf("%d\n", get_speed_correction());
    // delay(25);

    if (!is_competicion_iniciada()) {
      check_menu_button();
      if (get_start_btn()) {
        while (get_start_btn()) {
          set_RGB_color(255, 0, 0);
        }
        int32_t millisInicio = get_clock_ticks();
        int16_t millisPasados = 5;
        while (get_clock_ticks() < (millisInicio + MILLIS_INICIO)) {
          millisPasados = get_clock_ticks() - millisInicio;
          uint8_t r = 0, g = 0;
          r = map(millisPasados, 0, MILLIS_INICIO, 255, 0);
          g = map(millisPasados, 0, 1000, 0, 255);
          set_RGB_color(r, g, 0);
        }
        set_competicion_iniciada(true);
        set_RGB_color(0, 0, 0);
        resume_pid_timer();
        resume_speed_timer();
        set_speed(get_base_speed());
      }
    } else {
      // printf("%d\n", get_base_speed() + get_speed_correction());
      // delay(25);
    }

    debug_from_switch();
  }
}
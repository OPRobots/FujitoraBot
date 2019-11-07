
#include <buttons.h>
#include <config.h>
#include <control.h>
#include <debug.h>
#include <delay.h>
#include <encoders.h>
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
  update_encoder_readings();
}

int main(void) {
  setup();
  set_all_configs();

  calibrate_sensors();
  // int32_t last_micrometers_l = 0;
  // int32_t last_micrometers_r = 0;
  while (1) {
    // printf("%d\n", get_toggle_ticks());
    // delay(25);
    // printf("%d\n", get_speed_correction());
    // delay(25);

    if (!is_competicion_iniciada()) {
      check_menu_button();
      if (!in_debug_mode()) {
        if (get_start_btn()) {
          reset_menu_mode();
          set_status_led(false);
          while (get_start_btn()) {
            set_RGB_color(255, 0, 0);
          }
          int32_t millisInicio = get_clock_ticks();
          int16_t millisPasados = 5;
          while (get_clock_ticks() < (millisInicio + get_start_millis())) {
            millisPasados = get_clock_ticks() - millisInicio;
            uint8_t r = 0, g = 0;
            r = map(millisPasados, 0, get_start_millis(), 255, 0);
            g = map(millisPasados, 0, 1000, 0, 255);
            set_RGB_color(r, g, 0);
            if ((millisPasados > get_start_millis() * 0.75 || get_start_millis() == 0) && get_base_fan_speed() > 0) {
              set_fan_speed(get_base_fan_speed() * 0.75);
            }
          }
          set_competicion_iniciada(true);
          set_RGB_color(0, 0, 0);
          set_ideal_motors_speed(get_base_speed());
          set_ideal_motors_ms_speed(get_base_ms_speed());
          set_ideal_fan_speed(get_base_fan_speed());
          set_fan_speed(get_base_fan_speed());
          resume_pid_speed_timer();
        }
      }
    } else {
      // if (last_micrometers_r == 0 || last_micrometers_l == 0 || abs(last_micrometers_r - get_encoder_right_micrometers()) >= 10000 || abs(last_micrometers_l - get_encoder_left_micrometers()) >= 10000) {
      // last_micrometers_r = get_encoder_right_micrometers();
      // last_micrometers_l = get_encoder_left_micrometers();
      // printf("%ld\t%ld\n", get_encoder_x_position(), get_encoder_y_position());
      // delay(1);
      // }

      /* if (get_start_btn()) {
        set_ideal_motors_ms_speed(0.0);
        delay(100);
      }
      if (get_menu_down_btn()) {
        set_ideal_motors_ms_speed(1.0);
        delay(100);
      }
      if (get_menu_up_btn()) {
        set_ideal_motors_ms_speed(3.0);
        delay(100);
      }
      if (get_menu_mode_btn()) {
        set_ideal_motors_ms_speed(2.0);
        delay(100);
      }*/
      // debug_accel();
      // delay(20);
    }
  }
}
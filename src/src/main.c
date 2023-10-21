
#include "buttons.h"
#include "config.h"
#include "control.h"
#include "debug.h"
#include "delay.h"
#include "eeprom.h"
#include "encoders.h"
#include "leds.h"
#include "menu.h"
#include "motors.h"
#include "robotracer.h"
#include "sensors.h"
#include "setup.h"
#include "usart.h"

static bool use_btn_start = false;
static uint32_t millis_iniciado = 0;

void sys_tick_handler(void) {
  clock_tick();
  if (!is_esc_inited()) {
    init_esc();
  }
  update_encoder_readings();
  if (is_competicion_iniciada()) {
    update_log();
  }
}

/*
- [M] Encender robot
- [A] Cargar valores de EEPROM (sensores y menú) y asignarlos a sus variables correspondientes
- [M] Menú. Configurar/Comprobar velocidad y succión (estarán pre-cargados los valores confirmados anteriormente) y confirmar con BTN_START
- [M] Modo calibración.
- - [A] Si ya existen valores de calibración, se propone reutilizarlos (led rojo menu ON)
- - [M] Confirmar si usar valores de calibración existentes (led rojo menu ON) o calibrar de nuevo (led rojo menu OFF)
- - [A] Validar calibración de sensores (led rojo menu ON) & encoders al poner el robot en pista (todos sensores blanco)
- - [M?] Calibrar sensores (led rojo menu OFF) & comprobar encoders
- - [A/M] Si todo correcto, se muestra RGB verde por 1 segundo y se autoconfirma. En caso contrario, se queda en rojo y se confirma manual
- [A] Actualizar valores de EEPROM con calibración y menú
- [M] Modo menu again
- [M] Iniciar competición
*/

int main(void) {
  setup();
  eeprom_load();

  do {
    check_menu_button();
  } while (in_debug_mode() || !get_start_btn());
  do {
    reset_menu_mode();
    set_RGB_color(0, 0, 0);
  } while (get_start_btn());

  calibrate_sensors();

  eeprom_save();

  use_btn_start = get_swtich_3();
  millis_iniciado = 0;
  while (1) {
    if (!is_competicion_iniciada()) {
      check_menu_button();
      if (!in_debug_mode()) {
        if ((use_btn_start && get_start_btn()) || (!use_btn_start && get_swtich_3())) {
          reset_menu_mode();
          set_status_led(false);
          while (get_start_btn()) {
            set_RGB_color(255, 0, 0);
          }
          uint32_t millisInicio = get_clock_ticks();
          uint16_t millisPasados = 5;
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
          millis_iniciado = get_clock_ticks();
          set_RGB_color(0, 0, 0);
          set_ideal_motors_speed(get_base_speed());
          set_ideal_motors_ms_speed(get_base_ms_speed());
          set_acceleration_mss(get_base_acceleration_mss());
          set_deceleration_mss(get_base_deceleration_mss());
          set_ideal_fan_speed(get_base_fan_speed());
          set_fan_speed(get_base_fan_speed());
          if (get_config_track() == CONFIG_TRACK_ROBOTRACER) {
            robotracer_set_turn_speed(get_base_ms_speed());
            robotracer_set_straight_speed(get_robotracer_straight_ms_speed());
            robotracer_set_acceleration_mss(get_base_acceleration_mss());
            robotracer_set_deceleration_mss(get_base_deceleration_mss());
            robotracer_set_turn_acceleration_mss(get_base_turn_acceleration_mss());
            set_fans_speed(35, 35);
          }
          resume_pid_speed_timer();
        }
      }
    } else {

      if ((get_config_run() == CONFIG_RUN_DEBUG && get_clock_ticks() - millis_iniciado > 5000) || !get_swtich_3()) {
        emergency_stop();
      }
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

      /*if (get_menu_down_btn()) {
        left_mark();
        delay(500);
      }

      if (get_menu_up_btn()) {
        right_mark();
        delay(500);
      }*/
      // check_sector_radius();
      // delay(1);
      // debug_accel();
      // printf("%ld (%.2f - %.2f)\n", get_sensor_line_position(), get_encoder_left_speed(), get_encoder_right_speed());

      //   uint32_t data_to_write[] = {2500, 250, 25, 5};
      //   uint32_t data_readback[] = {0, 0, 0, 0};
      // while (1) {

      //   if (get_start_btn()) {
      //     while (get_start_btn()) {
      //     }
      //     set_status_led(true);
      //     eeprom_clear();
      //     set_status_led(false);
      //   }

      //   // Write data to the emulated EEPROM
      //   if (get_menu_up_btn()) {
      //     while (get_menu_up_btn()) {
      //     }
      //     set_status_led(true);
      //     eeprom_set_data(4, (uint32_t[]){3215, 125, 98, 138}, 4);
      //     eeprom_save();
      //     set_status_led(false);
      //   }

      //   if (get_menu_down_btn()) {
      //     while (get_menu_down_btn()) {
      //     }
      //     set_status_led(true);
      //     eeprom_set_data(0, (uint32_t[]){2500, 250, 25, 5}, 4);
      //     eeprom_save();
      //     set_status_led(false);
      //   }

      //   // Read data from the emulated EEPROM
      //   if (get_menu_mode_btn()) {
      //     while (get_menu_mode_btn()) {
      //     }
      //     uint32_t *data = eeprom_get_data();
      //     for (uint8_t i = 0; i < DATA_LENGTH; i++) {
      //       printf("%ld ", data[i]);
      //     }
      //     printf(" -> ");
      //     eeprom_load();
      //     for (uint8_t i = 0; i < DATA_LENGTH; i++) {
      //       printf("%ld ", data[i]);
      //     }
      //     printf("\n");
      //   }
      // }
    }
  }
}
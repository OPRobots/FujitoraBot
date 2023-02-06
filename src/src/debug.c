#include <debug.h>

bool debug_enabled = false;
uint32_t last_print_debug = 0;

/**
 * @brief Imprime los valores de los sensores sin aplicar ninguna corrección
 * 
 */
static void debug_sensors_raw() {
  if (get_clock_ticks() > last_print_debug + 50) {

    for (int8_t sensor = get_sensors_line_num() - 1; sensor >= 0; sensor--) {
      printf("%d\t", get_sensor_raw(sensor));
    }

    if (get_config_robot() == CONFIG_ROBOT_ROBOTRACER) {
      printf("\t");
      if (get_tipo_morro() == TIPO_MORRO_CORTO) {
        for (int8_t sensor = get_sensors_line_num(); sensor < get_sensors_num(); sensor++) {
          printf("%d\t", get_sensor_raw(sensor));
        }
      } else {
        for (int8_t sensor = get_sensors_line_num(); sensor < get_sensors_num() - 4; sensor++) {
          printf("%d\t", get_sensor_raw(sensor));
        }
      }
    }
    printf("\n");
    last_print_debug = get_clock_ticks();
  }
}

/**
 * @brief Imprime los valores de los sensores calibrándolos y escalandolos
 * 
 */
static void debug_sensors_calibrated() {
  if (get_clock_ticks() > last_print_debug + 50) {
    for (int8_t sensor = get_sensors_line_num() - 1; sensor >= 0; sensor--) {
      printf("%d\t", get_sensor_calibrated(sensor));
    }

    if (get_config_robot() == CONFIG_ROBOT_ROBOTRACER) {
      printf("\t");
      if (get_tipo_morro() == TIPO_MORRO_CORTO) {
        for (int8_t sensor = get_sensors_line_num(); sensor < get_sensors_num(); sensor++) {
          printf("%d\t", get_sensor_calibrated(sensor));
        }
      } else {
        for (int8_t sensor = get_sensors_line_num(); sensor < get_sensors_num() - 4; sensor++) {
          printf("%d\t", get_sensor_calibrated(sensor));
        }
      }
    }
    printf("\n");
    last_print_debug = get_clock_ticks();
  }
}

static void debug_all_leds() {
  set_RGB_rainbow();
  set_neon_heartbeat();
  warning_status_led(125);
}

static void debug_digital_io() {
  if (get_clock_ticks() > last_print_debug + 250) {
    printf("BTN: %d SW1: %d SW2: %d SW3: %d CFM: %d CFU: %d CFD: %d\n", get_start_btn(), get_swtich_1(), get_swtich_2(), get_swtich_3(), get_menu_mode_btn(), get_menu_up_btn(), get_menu_down_btn());
    last_print_debug = get_clock_ticks();
  }
}

static void debug_posicion_correccion() {
  if (get_clock_ticks() > last_print_debug + 50) {

    calc_sensor_line_position();
    float correccion_velocidad = calc_pid_correction(get_sensor_line_position());

    printf("%.3f - %ld\n", correccion_velocidad, get_sensor_line_position());
    last_print_debug = get_clock_ticks();
  }
}

static void debug_line_position() {
  if (get_clock_ticks() > last_print_debug + 50) {
    calc_sensor_line_position();
    printf("%d\t%ld\t%d\n", -(get_sensors_line_num() + 2) * 1000 / 2, get_sensor_line_position(), (get_sensors_line_num() + 2) * 1000 / 2);
    last_print_debug = get_clock_ticks();
  }
}

static void debug_encoders() {
  if (get_clock_ticks() > last_print_debug + 50) {
    printf("%ld (%ld)\t%ld (%ld)\n", get_encoder_left_total_ticks(), get_encoder_left_micrometers(), get_encoder_right_total_ticks(), get_encoder_right_micrometers());
    // printf("%.4f\t%.4f | %.4f\n", get_encoder_x_position(), get_encoder_y_position(), get_encoder_curernt_angle());
    // printf("%.4f | %.4f - %.4f\n", get_encoder_curernt_angle(),get_encoder_avg_micrometers(), get_encoder_angular_speed());
    last_print_debug = get_clock_ticks();
  }
}

static void debug_motors() {
  if (is_esc_inited()) {
    set_motors_speed(15, 15);
  }
}

static void debug_fans() {
  if (is_esc_inited()) {
    if (get_config_robot() == CONFIG_ROBOT_LINEFOLLOWER) {
      set_fan_speed(80);
    } else {
      set_fans_speed(50, 50);
    }
  }
}

static void check_debug_btn() {
  if (get_start_btn()) {
    debug_enabled = !debug_enabled;
    while (get_start_btn()) {
    }
    delay(50);
  }
  if (debug_enabled) {
    set_neon_heartbeat();
  } else {
    set_neon_fade(0);
  }
}

void debug_from_config(uint8_t type) {
  check_debug_btn();
  if (debug_enabled) {
    switch (type) {
      case DEBUG_TYPE_SENSORS_RAW:
        debug_sensors_raw();
        break;
      case DEBUG_TYPE_SENSORS_CALIBRATED:
        debug_sensors_calibrated();
        break;
      case DEBUG_TYPE_LINE_POSITION:
        debug_line_position();
        break;
      case DEBUG_TYPE_MOTORS:
        debug_motors();
        break;
      case DEBUG_TYPE_ENCODERS:
        debug_encoders();
        break;
      case DEBUG_TYPE_DIGITAL_IO:
        debug_digital_io();
        break;
      case DEBUG_TYPE_CORRECCION_POSICION:
        debug_posicion_correccion();
        break;
      case DEBUG_TYPE_LEDS_PARTY:
        debug_all_leds();
        break;
      case DEBUG_TYPE_FANS_DEMO:
        debug_fans();
        break;
    }
  } else {
    switch (type) {
      case DEBUG_TYPE_MOTORS:
        set_motors_speed(0, 0);
        break;
      case DEBUG_TYPE_LEDS_PARTY:
        all_leds_clear();
        break;
      case DEBUG_TYPE_FANS_DEMO:
        set_fan_speed(0);
        break;
    }
  }
}


void debug_sensors_calibration() {
  if (get_clock_ticks() > last_print_debug + 1000) {
    print_sensors_calibrations();
    last_print_debug = get_clock_ticks();
  }
}

void update_log(){
  
}

void debug_log(){

}
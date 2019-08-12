#include <debug.h>

void debug_sensors_raw() {
  for (uint8_t sensor = 0; sensor < get_sensors_num(); sensor++) {
    printf("%d\t", get_sensor_raw(sensor));
  }
  printf("\n");
  delay(50);
}

void debug_sensors_calibrated() {
  for (uint8_t sensor = 0; sensor < get_sensors_num(); sensor++) {
    printf("%d\t", get_sensor_calibrated(sensor));
  }
  printf("\n");
  delay(50);
}

void debug_all_leds() {
  set_RGB_rainbow();
  set_neon_heartbeat();
  warning_status_led(125);
}

void debug_digital_io() {
  printf("BTN: %d SW1: %d SW2: %d SW3: %d CFM: %d CFU: %d CFD: %d\n", get_start_btn(), get_swtich_1(), get_swtich_2(), get_swtich_3(), get_menu_mode_btn(), get_menu_up_btn(), get_menu_down_btn());
  delay(250);
}

void debug_line_position() {
  printf("%d\t%ld\t%d\n", -(get_sensors_line_num() + 2) * 1000 / 2, get_sensor_line_position(), (get_sensors_line_num() + 2) * 1000 / 2);
  delay(50);
}

void debug_motors() {
  if (is_esc_inited()) {
    set_motors_speed(15, -15);
    delay(1000);
    set_motors_speed(0, 0);
    delay(5000);
  }
}

void debug_from_switch() {
  if (get_switch_decimal() == 0) {
    return;
  }
  if (get_switch_decimal() != 7) {
    all_leds_clear();
  }
  switch (get_switch_decimal()) {
    case 1:
      debug_sensors_raw();
      break;
    case 2:
      debug_sensors_calibrated();
      break;
    case 3:
      debug_digital_io();
      break;
    case 4:
      debug_line_position();
      break;
    case 5:
      // TODO: añadir comprobación de encoders
      break;
    case 6:
      debug_motors();
      break;
    case 7:
      debug_all_leds();
      break;
  }
}
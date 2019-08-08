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
  gpio_clear(GPIOA, GPIO8 | GPIO9 | GPIO10 | GPIO11 | GPIO12);
  gpio_set(GPIOA, GPIO8);
  delay(250);

  gpio_clear(GPIOA, GPIO8 | GPIO9 | GPIO10 | GPIO11 | GPIO12);
  gpio_set(GPIOA, GPIO9);
  delay(250);

  gpio_clear(GPIOA, GPIO8 | GPIO9 | GPIO10 | GPIO11 | GPIO12);
  gpio_set(GPIOA, GPIO10);
  delay(250);

  gpio_clear(GPIOA, GPIO8 | GPIO9 | GPIO10 | GPIO11 | GPIO12);
  gpio_set(GPIOA, GPIO11);
  delay(250);

  gpio_clear(GPIOA, GPIO8 | GPIO9 | GPIO10 | GPIO11 | GPIO12);
  gpio_set(GPIOA, GPIO12);
  delay(250);
}

void debug_digital_io() {
  printf("BTN: %d SW1: %d SW2: %d SW3: %d CFM: %d CFU: %d CFD: %d\n", (bool)gpio_get(GPIOB, GPIO15), (bool)gpio_get(GPIOB, GPIO14), (bool)gpio_get(GPIOB, GPIO13), (bool)gpio_get(GPIOB, GPIO12), (bool)gpio_get(GPIOC, GPIO11), (bool)gpio_get(GPIOC, GPIO10), (bool)gpio_get(GPIOA, GPIO15));
  delay(250);
}

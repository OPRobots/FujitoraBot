
#include <delay.h>
#include <leds.h>
#include <sensors.h>
#include <setup.h>
#include <usart.h>

volatile uint16_t sensores_raw[16];

void sys_tick_handler(void) { clock_tick(); }

void test_xmas_leds() {
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

void test_entradas_digitales() {
  printf("BTN: %d SW1: %d SW2: %d SW3: %d CFM: %d CFU: %d CFD: %d\n", (bool)gpio_get(GPIOB, GPIO15), (bool)gpio_get(GPIOB, GPIO14), (bool)gpio_get(GPIOB, GPIO13), (bool)gpio_get(GPIOB, GPIO12), (bool)gpio_get(GPIOC, GPIO11), (bool)gpio_get(GPIOC, GPIO10), (bool)gpio_get(GPIOA, GPIO15));
  delay(250);
}

void test_sensores_adc1_dma() {
  for (uint8_t sensor = 0; sensor < get_sensors_num(); sensor++) {
    printf("%d\t", get_sensor_raw(sensor));
  }
  printf("\n");
  delay(50);
}

int main(void) {

  setup();
  // int last_ticks_crazy_blink = get_clock_ticks();
  while (1) {
    // set_neon_heartbeat();
    // set_rgb_rainbow();
    // if (get_clock_ticks() > last_ticks_crazy_blink + 50) {
    //   last_ticks_crazy_blink = get_clock_ticks();
    //   toggle_status_led();
    // }
    // timer_set_oc_value(TIM1, TIM_OC1, 0);
    // timer_set_oc_value(TIM1, TIM_OC2, 0);
    // timer_set_oc_value(TIM1, TIM_OC3, 0);
    // timer_set_oc_value(TIM1, TIM_OC4, 0);
    // delay(500);
    // timer_set_oc_value(TIM1, TIM_OC1, 50);
    // delay(500);
    // timer_set_oc_value(TIM1, TIM_OC1, 750);
    // timer_set_oc_value(TIM1, TIM_OC2, 750);
    // timer_set_oc_value(TIM1, TIM_OC3, 750);
    // timer_set_oc_value(TIM1, TIM_OC4, 750);
    // delay(500);
    // timer_set_oc_value(TIM1, TIM_OC1, 1000);
    // timer_set_oc_value(TIM1, TIM_OC2, 1000);
    // timer_set_oc_value(TIM1, TIM_OC3, 1000);
    // timer_set_oc_value(TIM1, TIM_OC4, 1000);
    // delay(500);
    //   // printf("probando...%d\n", get_clock_ticks());
    // test_entradas_digitales();
    // test_xmas_leds();
    test_sensores_adc1_dma();
  }
}

#include <delay.h>
#include <setup.h>
#include <usart.h>

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

int main(void) {

  setup();

  while (1) {
    // printf("probando...%d\n", get_clock_ticks());
    // test_entradas_digitales();
    // test_xmas_leds();
  }
}
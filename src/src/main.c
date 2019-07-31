
#include <delay.h>
#include <setup.h>
#include <usart.h>

void sys_tick_handler(void) { clock_tick(); }

int main(void) {

  setup();

  gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO12);

  gpio_set(GPIOA, GPIO12);

  while (1) {
    gpio_toggle(GPIOA, GPIO12);
    printf("probando...%d\n", get_clock_ticks());
    delay(250);
  }
}
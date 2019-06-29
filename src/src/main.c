
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>


int main(void) {
  // Blink Simple
  rcc_clock_setup_hse_3v3(&hse_16mhz_3v3[CLOCK_3V3_168MHZ]);

  rcc_periph_clock_enable(RCC_GPIOA);
  gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO1);

  gpio_set(GPIOA, GPIO1);
  while (1) {
    gpio_toggle(GPIOA, GPIO1);
   for (int i = 0; i < 6000000; i++) {
      __asm__("nop");
    }
  }
}
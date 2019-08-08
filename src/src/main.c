
#include <debug.h>
#include <delay.h>
#include <leds.h>
#include <sensors.h>
#include <setup.h>
#include <usart.h>


void sys_tick_handler(void) { clock_tick(); }

int main(void) {

  setup();

  while (1) {
  }
}
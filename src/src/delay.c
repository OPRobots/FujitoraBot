#include "delay.h"

static volatile uint32_t clock_ticks;

/**
 * @brief Aumenta el contador de pulsos del reloj
 * 
 */
void clock_tick(void) { clock_ticks++; }

/**
 * @brief Obtiene el contaor de ciclos de ejecución interno
 * 
 * @return uint32_t 
 */
uint32_t read_cycle_counter(void) {
  return dwt_read_cycle_counter();
}

/**
 * @brief Realiza una pausa en el programa
 * 
 * @param ms Tiempo en ms de la pausa
 */
void delay(uint32_t ms) {
  uint32_t awake = clock_ticks + ms;

  while (awake > clock_ticks)
    ;
}

/**
 * @brief Obtiene el número de pulsos de la ejecución
 * 
 * @return uint32_t 
 */
uint32_t get_clock_ticks(void) {
  return clock_ticks;
}

/**
 * @brief Realiza una pausa en el programa
 * 
 * @param us Tiempo en us de la pausa
 */
void delay_us(uint32_t us) {
  uint32_t initial_cycles = dwt_read_cycle_counter();
  uint32_t sleep_cycles = (uint32_t)(SYSCLK_FREQUENCY_HZ * ((float)us / (float)MICROSECONDS_PER_SECOND));

  while (dwt_read_cycle_counter() - initial_cycles <= sleep_cycles) {
    // __asm__("nop");
  };
}
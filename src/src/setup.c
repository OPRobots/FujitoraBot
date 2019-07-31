#include <setup.h>

/**
 * @brief Configura los relojes principales del robot
 *
 * SYSCLK a 168 MHz
 * GPIO: A, B, C
 * USART3
 * Timers: 1-PWM, 2-PID, 8-OneShot125
 * ADC
 * DWT
 *
 */
static void setup_clock() {
  rcc_clock_setup_hse_3v3(&hse_8mhz_3v3[CLOCK_3V3_168MHZ]);

  rcc_periph_clock_enable(RCC_GPIOA);
  rcc_periph_clock_enable(RCC_GPIOB);
  rcc_periph_clock_enable(RCC_GPIOC);

  rcc_periph_clock_enable(RCC_USART3);

  rcc_periph_clock_enable(RCC_DMA2);

  //   rcc_periph_clock_enable(RCC_TIM1);
  //   rcc_periph_clock_enable(RCC_TIM2);
  //   rcc_periph_clock_enable(RCC_TIM8);

  rcc_periph_clock_enable(RCC_ADC1);

  dwt_enable_cycle_counter();
}

/**
 * @brief Configura el SysTick para 1ms
 *
 */
static void setup_systick() {
  systick_set_frequency(SYSTICK_FREQUENCY_HZ, 168000000);
  systick_counter_enable();
  systick_interrupt_enable();
}

static void setup_timer_priorities() {
  nvic_set_priority(NVIC_SYSTICK_IRQ, 16 * 1);
  // nvic_set_priority(NVIC_DMA2_STREAM0_IRQ, 16 * 2);
  nvic_set_priority(NVIC_USART3_IRQ, 16 * 2);

  //   nvic_enable_irq(NVIC_TIM2_IRQ);
  nvic_enable_irq(NVIC_USART3_IRQ);
  // nvic_enable_irq(NVIC_DMA2_STREAM0_IRQ);
}

static void setup_usart(void) {
  // gpio_mode_setup(GPIOB, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO10);
  // gpio_mode_setup(GPIOB, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, GPIO11);
  
	/* Setup GPIO pins for USART2 transmit. */
	gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO10|GPIO11);

	/* Setup USART2 TX pin as alternate function. */
	gpio_set_af(GPIOB, GPIO_AF7, GPIO10|GPIO11);

  usart_set_baudrate(USART3, 115200);
  usart_set_databits(USART3, 8);
  usart_set_stopbits(USART3, USART_STOPBITS_1);
  usart_set_parity(USART3, USART_PARITY_NONE);
  usart_set_flow_control(USART3, USART_FLOWCONTROL_NONE);
  usart_set_mode(USART3, USART_MODE_TX_RX);

  // USART_CR1(USART3) |= USART_CR1_RXNEIE;
  // usart_enable_tx_interrupt(USART3);

  usart_enable(USART3);
}

void setup() {
  setup_clock();
  setup_usart();
  setup_timer_priorities();
  setup_systick();
}
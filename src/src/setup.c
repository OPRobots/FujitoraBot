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
  nvic_set_priority(NVIC_DMA2_STREAM0_IRQ, 16 * 2);
  nvic_set_priority(NVIC_USART3_IRQ, 16 * 3);

  //   nvic_enable_irq(NVIC_TIM2_IRQ);
  nvic_enable_irq(NVIC_USART3_IRQ);
  nvic_enable_irq(NVIC_DMA2_STREAM0_IRQ);
}

static void setup_usart(void) {
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

static void setup_gpio() {
  // Entradas analógicas sensores de línea
  gpio_mode_setup(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO0 | GPIO1 | GPIO2 | GPIO3 | GPIO4 | GPIO5 | GPIO6 | GPIO7);
  gpio_mode_setup(GPIOC, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO4 | GPIO5);
  gpio_mode_setup(GPIOB, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO0 | GPIO1);

  // Entradas analógicas sensores de marcas
  gpio_mode_setup(GPIOC, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO0 | GPIO1 | GPIO2 | GPIO3);

  // Entradas digitales configuracion
  gpio_mode_setup(GPIOC, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO10 | GPIO11);
  gpio_mode_setup(GPIOA, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO15);

  // Entradas digitales Switch y botón de inicio
  gpio_mode_setup(GPIOB, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO12 | GPIO13 | GPIO14 | GPIO15);

  // TODO: Entradas Encoders

  // Salida digital LED auxiliar
  gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO12);

  // Salida PWM LEDS
  gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO8 | GPIO9 | GPIO10 | GPIO11 | GPIO12);

  // Salida PWM Motores
  gpio_mode_setup(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO6 | GPIO7 | GPIO8 | GPIO9);

  // USART3
  gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO10 | GPIO11);
  gpio_set_af(GPIOB, GPIO_AF7, GPIO10 | GPIO11);
}

static void setup_adc1() {

  uint8_t sensores[16] = {ADC_CHANNEL0, ADC_CHANNEL1, ADC_CHANNEL2, ADC_CHANNEL3, ADC_CHANNEL4, ADC_CHANNEL5, ADC_CHANNEL6, ADC_CHANNEL7, ADC_CHANNEL14, ADC_CHANNEL15, ADC_CHANNEL8, ADC_CHANNEL9, ADC_CHANNEL10, ADC_CHANNEL11, ADC_CHANNEL12, ADC_CHANNEL13};

  adc_off(ADC1);
  adc_disable_external_trigger_regular(ADC1);
  adc_set_resolution(ADC1, ADC_CR1_RES_12BIT);
  adc_set_right_aligned(ADC1);
  // adc_set_clk_prescale(ADC_CCR_ADCPRE_BY2);
  adc_set_sample_time_on_all_channels(ADC1, ADC_SMPR_SMP_15CYC);
  adc_enable_scan_mode(ADC1);

  adc_set_regular_sequence(ADC1, 16, sensores);
  adc_set_continuous_conversion_mode(ADC1);
  adc_enable_eoc_interrupt(ADC1);

  adc_power_on(ADC1);
  int i;
  for (i = 0; i < 800000; i++) {
    /* Wait a bit. */
    __asm__("nop");
  }

  adc_start_conversion_regular(ADC1);
}

static void setup_dma_adc1(volatile uint16_t *sensor_raw) {
  rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_ADC1EN);
  rcc_peripheral_enable_clock(&RCC_AHB1ENR, RCC_AHB1ENR_DMA2EN);
  dma_stream_reset(DMA2, DMA_STREAM0);

  dma_set_peripheral_address(DMA2, DMA_STREAM0, (uint32_t)&ADC_DR(ADC1));
  dma_set_memory_address(DMA2, DMA_STREAM0, (uint32_t)sensor_raw);
  dma_enable_memory_increment_mode(DMA2, DMA_STREAM0);
  dma_set_peripheral_size(DMA2, DMA_STREAM0, DMA_SxCR_PSIZE_16BIT);
  dma_set_memory_size(DMA2, DMA_STREAM0, DMA_SxCR_MSIZE_16BIT);
  dma_set_priority(DMA2, DMA_STREAM0, DMA_SxCR_PL_LOW);

  dma_enable_transfer_complete_interrupt(DMA2, DMA_STREAM0);
  //dma_enable_half_transfer_interrupt(DMA2, DMA_STREAM0);
  dma_set_number_of_data(DMA2, DMA_STREAM0, 16);
  dma_enable_circular_mode(DMA2, DMA_STREAM0);
  dma_set_transfer_mode(DMA2, DMA_STREAM0, DMA_SxCR_DIR_PERIPHERAL_TO_MEM);
  dma_channel_select(DMA2, DMA_STREAM0, DMA_SxCR_CHSEL_0);

  dma_enable_stream(DMA2, DMA_STREAM0);
  adc_enable_dma(ADC1);
  adc_set_dma_continue(ADC1);
}
void dma2_stream0_isr(void) {
  if (dma_get_interrupt_flag(DMA2, DMA_STREAM0, DMA_TCIF)) {
    dma_clear_interrupt_flags(DMA2, DMA_STREAM0, DMA_TCIF);
  }
}

void setup() {
  setup_clock();
  setup_gpio();
  setup_usart();
  setup_timer_priorities();
  setup_systick();
}

void setup_adc1_dma(volatile uint16_t *sensor_raw) {
  setup_dma_adc1(sensor_raw);
  setup_adc1();
}
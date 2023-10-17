#include "setup.h"

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
static void setup_clock(void) {
  rcc_clock_setup_pll(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_168MHZ]);

  rcc_periph_clock_enable(RCC_GPIOA);
  rcc_periph_clock_enable(RCC_GPIOB);
  rcc_periph_clock_enable(RCC_GPIOC);

  rcc_periph_clock_enable(RCC_USART3);

  rcc_periph_clock_enable(RCC_DMA2);

  rcc_periph_clock_enable(RCC_TIM1);
  rcc_periph_clock_enable(RCC_TIM2);
  rcc_periph_clock_enable(RCC_TIM3);
  rcc_periph_clock_enable(RCC_TIM4);
  rcc_periph_clock_enable(RCC_TIM5);
  rcc_periph_clock_enable(RCC_TIM8);

  rcc_periph_clock_enable(RCC_ADC1);

  dwt_enable_cycle_counter();
}

/**
 * @brief Configura el SysTick para 1ms
 *
 */
static void setup_systick(void) {
  systick_set_frequency(SYSTICK_FREQUENCY_HZ, 168000000);
  systick_counter_enable();
  systick_interrupt_enable();
}

static void setup_timer_priorities(void) {
  nvic_set_priority(NVIC_SYSTICK_IRQ, 16 * 1);
  nvic_set_priority(NVIC_DMA2_STREAM0_IRQ, 16 * 2);
  nvic_set_priority(NVIC_TIM2_IRQ, 16 * 3);
  nvic_set_priority(NVIC_TIM5_IRQ, 16 * 4);
  nvic_set_priority(NVIC_USART3_IRQ, 16 * 5);

  //   nvic_enable_irq(NVIC_TIM5_IRQ);
  nvic_enable_irq(NVIC_DMA2_STREAM0_IRQ);
  nvic_enable_irq(NVIC_TIM5_IRQ);
  nvic_enable_irq(NVIC_TIM2_IRQ);
  nvic_enable_irq(NVIC_USART3_IRQ);
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

static void setup_gpio(void) {
  // Entradas digitales configuracion
  gpio_mode_setup(GPIOC, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO10 | GPIO11);
  gpio_mode_setup(GPIOA, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO15);  
  set_all_configs();

  if (get_config_robot() == CONFIG_ROBOT_LINEFOLLOWER) {
    // Entradas analógicas sensores de línea
    gpio_mode_setup(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO0 | GPIO1 | GPIO2 | GPIO3 | GPIO4 | GPIO5 | GPIO6 | GPIO7);
    gpio_mode_setup(GPIOC, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO4 | GPIO5);
    gpio_mode_setup(GPIOB, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO0 | GPIO1);
  } else {
    // Entradas analógicas sensores de línea
    gpio_mode_setup(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO2 | GPIO3 | GPIO4 | GPIO5 | GPIO6 | GPIO7);
    gpio_mode_setup(GPIOC, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO4 | GPIO5);
    gpio_mode_setup(GPIOB, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO0 | GPIO1);

    // Salidas digitales de conmutación de morros
    gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO0 | GPIO1);
  }

  // Entradas analógicas sensores de marcas
  gpio_mode_setup(GPIOC, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO0 | GPIO1 | GPIO2 | GPIO3);


  // Entradas digitales Switch y botón de inicio
  gpio_mode_setup(GPIOB, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO12 | GPIO13 | GPIO14 | GPIO15);

  // Entradas Encoders
  gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO4 | GPIO5 | GPIO6 | GPIO7);
  gpio_set_af(GPIOB, GPIO_AF2, GPIO4 | GPIO5 | GPIO6 | GPIO7);

  // Salida digital LED auxiliar
  gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO12);

  // Salida PWM LEDS
  gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO8 | GPIO9 | GPIO10 | GPIO11);
  gpio_set_af(GPIOA, GPIO_AF1, GPIO8 | GPIO9 | GPIO10 | GPIO11);

  // Salida PWM Motores
  gpio_mode_setup(GPIOC, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO6 | GPIO7 | GPIO8 | GPIO9);
  gpio_set_af(GPIOC, GPIO_AF3, GPIO6 | GPIO7 | GPIO8 | GPIO9);

  // USART3
  gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO10 | GPIO11);
  gpio_set_af(GPIOB, GPIO_AF7, GPIO10 | GPIO11);
}

static void setup_adc1(void) {
  adc_power_off(ADC1);
  adc_disable_external_trigger_regular(ADC1);
  adc_set_resolution(ADC1, ADC_CR1_RES_12BIT);
  adc_set_right_aligned(ADC1);
  // adc_set_clk_prescale(ADC_CCR_ADCPRE_BY2);
  adc_set_sample_time_on_all_channels(ADC1, ADC_SMPR_SMP_15CYC);
  adc_enable_scan_mode(ADC1);

  adc_set_regular_sequence(ADC1, get_sensors_num(), get_sensors());
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

static void setup_dma_adc1(void) {
  rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_ADC1EN);
  rcc_peripheral_enable_clock(&RCC_AHB1ENR, RCC_AHB1ENR_DMA2EN);
  dma_stream_reset(DMA2, DMA_STREAM0);

  dma_set_peripheral_address(DMA2, DMA_STREAM0, (uint32_t)&ADC_DR(ADC1));
  dma_set_memory_address(DMA2, DMA_STREAM0, (uint32_t)get_sensors_raw());
  dma_enable_memory_increment_mode(DMA2, DMA_STREAM0);
  dma_set_peripheral_size(DMA2, DMA_STREAM0, DMA_SxCR_PSIZE_16BIT);
  dma_set_memory_size(DMA2, DMA_STREAM0, DMA_SxCR_MSIZE_16BIT);
  dma_set_priority(DMA2, DMA_STREAM0, DMA_SxCR_PL_LOW);

  dma_enable_transfer_complete_interrupt(DMA2, DMA_STREAM0);
  //dma_enable_half_transfer_interrupt(DMA2, DMA_STREAM0);
  dma_set_number_of_data(DMA2, DMA_STREAM0, get_sensors_num());
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

static void setup_leds_pwm(void) {
  timer_set_mode(TIM1, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);

  timer_set_prescaler(TIM1, rcc_apb2_frequency * 2 / 400000);
  // 400000 es la frecuencia a la que irá el PWM 4 kHz, los dos últimos ceros no se porqué, pero son necesarios ya que rcc_apb2_frequency también añade dos ceros a mayores
  timer_set_repetition_counter(TIM1, 0);
  timer_enable_preload(TIM1);
  timer_continuous_mode(TIM1);
  timer_set_period(TIM1, LEDS_MAX_PWM);

  timer_set_oc_mode(TIM1, TIM_OC1, TIM_OCM_PWM1);
  timer_set_oc_mode(TIM1, TIM_OC2, TIM_OCM_PWM1);
  timer_set_oc_mode(TIM1, TIM_OC3, TIM_OCM_PWM1);
  timer_set_oc_mode(TIM1, TIM_OC4, TIM_OCM_PWM1);
  timer_set_oc_value(TIM1, TIM_OC1, 0);
  timer_set_oc_value(TIM1, TIM_OC2, 0);
  timer_set_oc_value(TIM1, TIM_OC3, 0);
  timer_set_oc_value(TIM1, TIM_OC4, 0);
  timer_enable_oc_output(TIM1, TIM_OC1);
  timer_enable_oc_output(TIM1, TIM_OC2);
  timer_enable_oc_output(TIM1, TIM_OC3);
  timer_enable_oc_output(TIM1, TIM_OC4);

  timer_enable_break_main_output(TIM1);

  timer_enable_counter(TIM1);
}

static void setup_motors_pwm(void) {
  timer_set_mode(TIM8, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);

  //84000000
  timer_set_prescaler(TIM8, rcc_apb2_frequency * 2 / 4000000 - 2);
  // 4000000 es la frecuencia a la que irá el PWM 4 kHz, los dos últimos ceros no se porqué, pero son necesarios ya que rcc_apb2_frequency también añade dos ceros a mayores
  timer_set_repetition_counter(TIM8, 0);
  timer_enable_preload(TIM8);
  timer_continuous_mode(TIM8);
  timer_set_period(TIM8, MOTORES_MAX_PWM);

  timer_set_oc_mode(TIM8, TIM_OC1, TIM_OCM_PWM1);
  timer_set_oc_mode(TIM8, TIM_OC2, TIM_OCM_PWM1);
  timer_set_oc_mode(TIM8, TIM_OC3, TIM_OCM_PWM1);
  timer_set_oc_mode(TIM8, TIM_OC4, TIM_OCM_PWM1);

  timer_enable_break_main_output(TIM8);

  timer_enable_counter(TIM8);
}

static void setup_pid_speed_timer(void) {
  rcc_periph_reset_pulse(RST_TIM5);
  timer_set_mode(TIM5, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
  timer_set_prescaler(TIM5, ((rcc_apb1_frequency * 2) / 1000000 - 2));
  timer_disable_preload(TIM5);
  timer_continuous_mode(TIM5);
  timer_set_period(TIM5, 1024);

  timer_enable_counter(TIM5);
  // El timer se iniciará en el arranque
  // timer_enable_irq(TIM5, TIM_DIER_CC1IE);
}

void tim5_isr(void) {
  if (timer_get_flag(TIM5, TIM_SR_CC1IF)) {
    timer_clear_flag(TIM5, TIM_SR_CC1IF);
    pid_speed_timer_custom_isr();
  }
}

static void setup_quadrature_encoders(void) {
  timer_set_period(TIM4, 0xFFFF);
  timer_slave_set_mode(TIM4, TIM_SMCR_SMS_EM3);
  timer_ic_set_input(TIM4, TIM_IC1, TIM_IC_IN_TI1);
  timer_ic_set_input(TIM4, TIM_IC2, TIM_IC_IN_TI2);
  timer_enable_counter(TIM4);

  timer_set_period(TIM3, 0xFFFF);
  timer_slave_set_mode(TIM3, TIM_SMCR_SMS_EM3);
  timer_ic_set_input(TIM3, TIM_IC1, TIM_IC_IN_TI1);
  timer_ic_set_input(TIM3, TIM_IC2, TIM_IC_IN_TI2);
  timer_enable_counter(TIM3);
}

void setup(void) {
  setup_clock();
  setup_gpio();
  setup_usart();
  setup_timer_priorities();
  setup_leds_pwm();
  setup_motors_pwm();
  setup_dma_adc1();
  setup_adc1();
  setup_pid_speed_timer();
  setup_quadrature_encoders();
  setup_systick();
}
#include <motors.h>

static bool escInited = false;
static uint32_t escInitMillis = 0;
static const uint16_t STOP_MOTORS_PWM = (uint16_t)(MOTORES_MAX_PWM * 0.75);
static bool iniciado = false;
static uint32_t millis_iniciado = 0;

bool is_esc_inited() {
  return escInited;
}

void init_esc() {
  if (!escInited) {
    if (escInitMillis == 0) {
      escInitMillis = get_clock_ticks();

      timer_enable_oc_output(TIM8, TIM_OC1);
      timer_enable_oc_output(TIM8, TIM_OC2);
      timer_enable_oc_output(TIM8, TIM_OC3);
      timer_enable_oc_output(TIM8, TIM_OC4);
    }
    if (get_clock_ticks() - escInitMillis < 5000) {
      timer_set_oc_value(TIM8, TIM_OC1, STOP_MOTORS_PWM);
      timer_set_oc_value(TIM8, TIM_OC2, STOP_MOTORS_PWM);
      timer_set_oc_value(TIM8, TIM_OC3, STOP_MOTORS_PWM);
      timer_set_oc_value(TIM8, TIM_OC4, STOP_MOTORS_PWM);
    } else {
      escInited = true;
    }
  }
}

void set_motors_speed(int8_t velI, int8_t velD) {
  if (!escInited) {
    return;
  }
  if (velI == 0 && velD == 0) {
    iniciado = false;
  } else {
    if(!iniciado){
      iniciado = true;
      millis_iniciado = get_clock_ticks();
    }
  }
  uint16_t ocI = STOP_MOTORS_PWM;
  uint16_t ocD = STOP_MOTORS_PWM;

  if (velI != 0) {
    if (velI > 0) {
      ocI += map(velI, 0, 100, 0, (int32_t)(MOTORES_MAX_PWM * 0.25));
    } else {
      ocI -= map(abs(velI), 0, 100, 0, (int32_t)(MOTORES_MAX_PWM * 0.25));
    }
  }

  if (velD != 0) {
    if (velD > 0) {
      ocD += map(velD, 0, 100, 0, (int32_t)(MOTORES_MAX_PWM * 0.25));
    } else {
      ocD -= map(abs(velD), 0, 100, 0, (int32_t)(MOTORES_MAX_PWM * 0.25));
    }
  }
  timer_set_oc_value(TIM8, TIM_OC4, ocI);
  timer_set_oc_value(TIM8, TIM_OC2, ocD);
}
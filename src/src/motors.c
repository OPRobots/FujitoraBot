#include "motors.h"

static bool escInited = false;
static uint32_t escInitMillis = 0;
static const uint16_t STOP_MOTORS_PWM = (uint16_t)(MOTORES_MAX_PWM * 0.75);
static bool iniciado = false;
static uint32_t millis_iniciado = 0;

bool is_esc_inited(void) {
  return escInited;
}

void init_esc(void) {
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

void set_motors_speed(float velI, float velD) {
  if (!escInited) {
    return;
  }
  if (velI == 0 && velD == 0) {
    iniciado = false;
  } else {
    if (!iniciado) {
      iniciado = true;
      millis_iniciado = get_clock_ticks();
    }
  }
  float ocI = STOP_MOTORS_PWM;
  float ocD = STOP_MOTORS_PWM;

  if (velI != 0) {
    if (velI > 0) {
      ocI += map(velI, 0, 100, 0, (MOTORES_MAX_PWM * 0.25));
    } else {
      ocI -= map(abs(velI), 0, 100, 0, (MOTORES_MAX_PWM * 0.25));
    }
  }

  if (velD != 0) {
    if (velD > 0) {
      ocD += map(velD, 0, 100, 0, (MOTORES_MAX_PWM * 0.25));
    } else {
      ocD -= map(abs(velD), 0, 100, 0, (MOTORES_MAX_PWM * 0.25));
    }
  }
  timer_set_oc_value(TIM8, TIM_OC4, (uint32_t)ocI);
  timer_set_oc_value(TIM8, TIM_OC2, (uint32_t)ocD);
}

void set_fan_speed(uint8_t vel) {
  if (!escInited) {
    return;
  }

  uint32_t ocF = STOP_MOTORS_PWM;
  if (vel != 0) {
    if (vel > 0) {
      ocF -= map(vel, 0, 100, 0, (MOTORES_MAX_PWM * 0.25));
    } else {
      ocF += map(abs(vel), 0, 100, 0, (MOTORES_MAX_PWM * 0.25));
    }
  }
  timer_set_oc_value(TIM8, TIM_OC1, ocF);
  timer_set_oc_value(TIM8, TIM_OC3, ocF);
}

void set_fans_speed(int8_t velI, int8_t velD) {
  if (!escInited) {
    return;
  }

  uint32_t ocF_I = STOP_MOTORS_PWM;
  if (velI != 0) {
    if (velI > 0) {
      ocF_I += map(velI, 0, 100, 0, (MOTORES_MAX_PWM * 0.25));
    } else {
      ocF_I -= map(abs(velI), 0, 100, 0, (MOTORES_MAX_PWM * 0.25));
    }
  }
  uint32_t ocF_D = STOP_MOTORS_PWM;
  if (velD != 0) {
    if (velD > 0) {
      ocF_D -= map(velD, 0, 100, 0, (MOTORES_MAX_PWM * 0.25));
    } else {
      ocF_D += map(abs(velD), 0, 100, 0, (MOTORES_MAX_PWM * 0.25));
    }
  }
  timer_set_oc_value(TIM8, TIM_OC1, ocF_D);
  timer_set_oc_value(TIM8, TIM_OC3, ocF_I);
}
#include <control.h>

static float velocidad = 0;
static int32_t velocidadIdeal = 0;
static float velocidadIdealMs = 0.0;
static float velocidadObjetivoMs = 0.0;
static int32_t velocidadVentiladorIdeal = 0;
static bool competicionIniciada = false;
volatile static float correccion_velocidad = 0;
volatile static float error_anterior = 0;
volatile static float suma_error_ms = 0;
volatile static float error_anterior_ms = 0;
// static bool vuelta_iniciada = false;
static bool left_mark = false;
static bool right_mark = false;

static uint32_t resume_speed_ms = 0;

float calc_pid_correction(int32_t posicion) {
  float p = 0;
  float i = 0;
  float d = 0;
  int32_t error = 0 - posicion;

  p = KP * error;
  d = KD * (error - error_anterior);
  error_anterior = error;

  return p + i + d;
}

float calc_ms_pid_correction(float velocidadActualMs) {
  float p = 0;
  float i = 0;
  float d = 0;
  float error_ms = velocidadObjetivoMs - velocidadActualMs;

  if (velocidadObjetivoMs <= 1.0 && velocidadActualMs < velocidadIdealMs && velocidadIdealMs > 0) {
    if (KI_MS * suma_error_ms < 20.0) {
      set_status_led(false);
      suma_error_ms += error_ms;
    } else {
      set_status_led(true);
    }
  } else {
    suma_error_ms += error_ms;
    set_status_led(false);
  }
  // p = KP_MS * error_ms;
  i = KI_MS * suma_error_ms;
  // d = KD_MS * (error_ms - error_anterior_ms);
  error_anterior_ms = error_ms;

  return p + i + d;
}

bool is_competicion_iniciada() {
  return competicionIniciada;
}
void set_competicion_iniciada(bool state) {
  competicionIniciada = state;
}

void pid_speed_timer_custom_isr() {
  calc_sensor_line_position();
  correccion_velocidad = calc_pid_correction(get_sensor_line_position());
  check_side_marks();
  // if (!right_mark && is_right_mark()) {
  //   if (!vuelta_iniciada) {
  //     vuelta_iniciada = true;
  //     set_status_led(true);
  //   } else {
  //     vuelta_iniciada = false;
  //     all_leds_clear();
  //     set_competicion_iniciada(false);
  //     pause_pid_speed_timer();
  //   }
  // }

  // if (is_left_mark() || is_right_mark()) {
  //   set_neon_fade(1024);
  // } else {
  //   set_neon_fade(0);
  // }
  left_mark = is_left_mark();
  right_mark = is_right_mark();

  if (is_competicion_iniciada()) {
    if (get_config_speed() == CONFIG_SPEED_MS) {
      if (velocidadIdealMs > 0 || velocidadObjetivoMs > 0 || velocidad > 0) {
        if (velocidadObjetivoMs < velocidadIdealMs) {
          if (get_encoder_avg_speed() >= 1) {
            velocidadObjetivoMs += MAX_ACCEL_MS2 / 1000.0;
          } else {
            velocidadObjetivoMs += MIN_ACCEL_MS2 / 1000.0;
          }
        } else if (velocidadObjetivoMs > velocidadIdealMs) {
          if (get_encoder_avg_speed() >= 1) {
            velocidadObjetivoMs -= MAX_ACCEL_MS2 / 1000.0;
          } else {
            velocidadObjetivoMs -= MIN_ACCEL_MS2 / 1000.0;
          }
        }
        if (velocidadIdealMs != velocidadObjetivoMs && abs(velocidadIdealMs * 100 - velocidadObjetivoMs * 100) < 2) {
          velocidadObjetivoMs = velocidadIdealMs;
        }
        velocidad = calc_ms_pid_correction(get_encoder_avg_speed());
        if (velocidad > 100) {
          velocidad = 100;
        } else if (velocidad < 0) {
          velocidad = 0;
        }
        /*if (get_clock_ticks() % 20 == 0) {
          printf("%.2f\t%.2f\t%.2f\t%.2f\n", velocidadIdealMs, velocidadObjetivoMs, get_encoder_avg_speed(), velocidad / 10.0f);
        }*/
      } else {
        calc_ms_pid_correction(get_encoder_avg_speed());
        velocidad = 0;
        set_motors_speed(0, 0);
        return;
      }
    } else {
      if (velocidadIdeal > 0) {
        if (velocidad < velocidadIdeal) {
          if (velocidad < MIN_SPEED_PERCENT) {
            velocidad = MIN_SPEED_PERCENT;
          }
          float increment_percent = ((get_clock_ticks() - resume_speed_ms) / 1000.0) * MAX_ACCEL_PERCENT;
          velocidad = MIN_SPEED_PERCENT + increment_percent;
        } else if (velocidad != velocidadIdeal) {
          velocidad = velocidadIdeal;
        }
      } else {
        velocidad = 0;
        set_motors_speed(0, 0);
        return;
      }
    }

    float velI = velocidad - correccion_velocidad;
    float velD = velocidad + correccion_velocidad;

    if (velD < MIN_SPEED_PERCENT) {
      velI += MIN_SPEED_PERCENT - velD;
      velD = MIN_SPEED_PERCENT;
    } else if (velD > 100) {
      velD = 100;
    }
    if (velI < MIN_SPEED_PERCENT) {
      velD += MIN_SPEED_PERCENT - velI;
      velI = MIN_SPEED_PERCENT;
    } else if (velI > 100) {
      velI = 100;
    }
    set_motors_speed(velD, velI);
    set_fan_speed(velocidadVentiladorIdeal);

  } else {
    velocidad = 0;
    set_motors_speed(0, 0);
  }
}

float get_speed_correction() {
  return correccion_velocidad;
}

void set_ideal_motors_speed(int32_t v) {
  velocidadIdeal = v;
}

void set_ideal_motors_ms_speed(float ms) {
  velocidadIdealMs = ms;
}

void set_ideal_fan_speed(int32_t v) {
  velocidadVentiladorIdeal = v;
}

void resume_pid_speed_timer() {
  resume_speed_ms = get_clock_ticks();
  velocidad = 0;
  timer_enable_irq(TIM5, TIM_DIER_CC1IE);
}

void pause_pid_speed_timer() {
  timer_disable_irq(TIM5, TIM_DIER_CC1IE);
  correccion_velocidad = 0;
  set_motors_speed(0, 0);
  set_fan_speed(0);
  all_leds_clear();
}
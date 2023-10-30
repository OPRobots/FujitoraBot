#include "control.h"

static float velocidad = 0;
static int32_t velocidadIdeal = 0;
static float velocidadIdealMs = 0.0;
static float velocidadObjetivoMs = 0.0;
static float accelerationMss = 0.0;
static float decelerationMss = 0.0;
static int32_t velocidadVentiladorIdeal = 0;
static bool fullVelocidadVentilador = false;
static bool competicionIniciada = false;
static uint32_t competicionIniciadaMillis = 0;
static volatile float correccion_velocidad = 0;
static volatile float error_anterior = 0;
static volatile float suma_error_ms = 0;
static volatile float error_anterior_ms = 0;

static uint32_t resume_speed_ms = 0;

/**
 * @brief Calcula la corrección de posición en línea mediante PID
 * 
 * @param posicion Posición actual actual del robot sobre la línea
 * 
 * @return float Corrección aplicada sobre la velocidad
 */

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

/**
 * @brief Calcula la corrección de la velocidad en m/s mediante PID
 * 
 * @param velocidadActualMs Velocidad actual en m/s obtenida de los encoders
 * 
 * @return float Velocidad aplicada a los motores en %
 */

float calc_ms_pid_correction(float velocidadActualMs) {
  float p = 0;
  float i = 0;
  float d = 0;
  float error_ms = velocidadObjetivoMs - velocidadActualMs;

  if (velocidadObjetivoMs <= 1.0 && velocidadActualMs < velocidadIdealMs && velocidadIdealMs > 0) {
    if (KI_MS * suma_error_ms < 20.0) {
      suma_error_ms += error_ms;
    } else {
    }
  } else {
    suma_error_ms += error_ms;
  }
  // p = KP_MS * error_ms;
  i = KI_MS * suma_error_ms;
  // d = KD_MS * (error_ms - error_anterior_ms);
  error_anterior_ms = error_ms;

  return p + i + d;
}

/**
 * @brief Comprueba si el robot está en funcionamiento
 * 
 * @return bool 
 */

bool is_competicion_iniciada(void) {
  return competicionIniciada;
}

/**
 * @brief Establece el estado actual del robot
 * 
 * @param state Estado actual del robot
 */

void set_competicion_iniciada(bool state) {
  competicionIniciada = state;
  if (state) {
    competicionIniciadaMillis = get_clock_ticks();
  } else {
    competicionIniciadaMillis = 0;
  }
}

uint32_t get_competicion_iniciada_millis(void) {
  return competicionIniciadaMillis;
}

/**
 * @brief Función ISR del Timer5 encargada del control de posición, velocidad y mapeo de pista en caso de robotracer
 * 
 */
void pid_speed_timer_custom_isr(void) {
  calc_sensor_line_position();
  correccion_velocidad = calc_pid_correction(get_sensor_line_position());

  if (get_config_track() == CONFIG_TRACK_ROBOTRACER) {
    robotracer_loop_flow();
  }

  if (is_competicion_iniciada()) {
    if (get_config_speed() == CONFIG_SPEED_MS) {
      if (velocidadIdealMs > 0 || velocidadObjetivoMs > 0 || velocidad > 0) {
        if (velocidadObjetivoMs < velocidadIdealMs) {
          if (velocidadObjetivoMs < 1 && velocidadIdealMs > 0) {
            velocidadObjetivoMs += MIN_ACCEL_MS2 / 1000.0;
          } else {
            velocidadObjetivoMs += accelerationMss / 1000.0;
          }
        } else if (velocidadObjetivoMs > velocidadIdealMs) {
          if (velocidadObjetivoMs < 1 && velocidadIdealMs > 0) {
            velocidadObjetivoMs -= MIN_ACCEL_MS2 / 1000.0;
          } else {
            velocidadObjetivoMs -= decelerationMss / 1000.0;
          }
        }
        if (velocidadIdealMs != velocidadObjetivoMs && abs(velocidadIdealMs * 100 - velocidadObjetivoMs * 100) < 2) {
          velocidadObjetivoMs = velocidadIdealMs;
        }
        if (get_encoder_avg_speed() > 0.25) {
          velocidad = MIN_SPEED_PERCENT + calc_ms_pid_correction(get_encoder_avg_speed());
          if (get_encoder_avg_speed() > 0.5) {
            fullVelocidadVentilador = true;
          }
        } else {
          velocidad = 22;
          suma_error_ms = 0;
        }
        if (velocidad > 100) {
          velocidad = 100;
        } else if (velocidad < 0) {
          suma_error_ms = 0;
          velocidad = 0;
        }
        // if (get_clock_ticks() % 20 == 0) {
        //   debug_accel();
        // }
      } else {
        velocidad = 0;
        set_motors_speed(0, 0);
        set_fans_speed(0, 0);
        return;
      }
    } else {
      if (velocidadIdeal > 0) {
        if (velocidad < velocidadIdeal) {
          if (velocidad < MIN_SPEED_PERCENT) {
            velocidad = 15;
          }
          float increment_percent = ((get_clock_ticks() - resume_speed_ms) / 1000.0) * MAX_ACCEL_PERCENT;
          velocidad = 15 + increment_percent;
        } else if (velocidad != velocidadIdeal) {
          velocidad = velocidadIdeal;
        }
        if (velocidad > 30) {
          fullVelocidadVentilador = true;
        }
      } else {
        velocidad = 0;
        set_motors_speed(0, 0);
        set_fans_speed(0, 0);
        return;
      }
    }

    float velI = velocidad - correccion_velocidad;
    float velD = velocidad + correccion_velocidad;

    if (velD < MIN_SPEED_PERCENT) {
      // velI += MIN_SPEED_PERCENT - velD;
      velD = MIN_SPEED_PERCENT;
    } else if (velD > 100) {
      // velI -= velD - 100;
      velD = 100;
    }
    if (velI < MIN_SPEED_PERCENT) {
      // velD += MIN_SPEED_PERCENT - velI;
      velI = MIN_SPEED_PERCENT;
    } else if (velI > 100) {
      // velD -= velI - 100;
      velI = 100;
    }
    set_motors_speed(velD, velI);
    if (get_config_track() == CONFIG_TRACK_LINEFOLLOWER) {
      set_fan_speed(fullVelocidadVentilador ? velocidadVentiladorIdeal : (velocidadVentiladorIdeal / 2));
    }

  } else {
    velocidad = 0;
    set_motors_speed(0, 0);
  }
}

/**
 * @brief Obtiene la corrección de la Velocidad mediante PID para tareas de depuración
 * 
 * @return float 
 */
float get_speed_correction(void) {
  return correccion_velocidad;
}

/**
 * @brief Establece la velocidad ideal de los motores desde configuración
 * 
 * @param v Velocidad en % de PWM
 */
void set_ideal_motors_speed(int32_t v) {
  velocidadIdeal = v;
}

/**
 * @brief Establece la velocidad ideal de los motores desde configuración
 * 
 * @param ms Velocidad en m/s
 */
void set_ideal_motors_ms_speed(float ms) {
  velocidadIdealMs = ms;
}

/**
 * @brief Establece la aceleración máxima al incrementar velocidad
 * 
 * @param mss Aceleración en m/s/s
 */
void set_acceleration_mss(float mss) {
  accelerationMss = mss;
}

/**
 * @brief Establece la aceleración máxima al reducir velocidad
 * 
 * @param mss Aceleración en m/s/s
 */
void set_deceleration_mss(float mss) {
  decelerationMss = mss;
}

/**
 * @brief Establece la velocidad del ventilador de succión / soplar
 * 
 * @param v Velocidad en % de PWM
 */
void set_ideal_fan_speed(int32_t v) {
  velocidadVentiladorIdeal = v;
}

/**
 * @brief Reanuda la ejecución del Timer encargado del PID
 * 
 */
void resume_pid_speed_timer(void) {
  resume_speed_ms = get_clock_ticks();
  velocidad = 0;
  correccion_velocidad = 0;
  velocidadObjetivoMs = 0;
  fullVelocidadVentilador = false;
  if (get_config_track() == CONFIG_TRACK_ROBOTRACER) {
    robotracer_restart();
  }
  timer_enable_irq(TIM5, TIM_DIER_CC1IE);
}

/**
 * @brief Pausa la ejecución del Timer encargado del PID
 * 
 */
void pause_pid_speed_timer(void) {
  timer_disable_irq(TIM5, TIM_DIER_CC1IE);
  set_motors_speed(0, 0);
  all_leds_clear();
}

/**
 * @brief Imprime valores clave para depurar la aceleración
 * 
 */
void debug_accel(void) {
  printf("%.2f\t%.2f\t%.2f\t%.2f\n", velocidadIdealMs, velocidadObjetivoMs, get_encoder_avg_speed(), velocidad / 10.0f);
}
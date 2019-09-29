#include <control.h>

static float velocidad = 0;
static int32_t velocidadIdeal = 0;
static int32_t velocidadVentiladorIdeal = 0;
static bool competicionIniciada = false;
volatile static float correccion_velocidad = 0;
volatile static int32_t error_anterior = 0;
// static bool vuelta_iniciada = false;
static bool left_mark = false;
static bool right_mark = false;

static uint32_t resume_speed_ms = 0;

static float calc_pid_correction(int32_t posicion) {
  float p = 0;
  float i = 0;
  float d = 0;
  int32_t error = 0 - posicion;

  p = KP * error;
  d = KD * (error - error_anterior);
  error_anterior = error;

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
      //TODO: asignar velocidad mediante m/s
    } else {
      if (velocidadIdeal > 0) {
        if (velocidad < velocidadIdeal) {
          if (velocidad < 10) {
            velocidad = 10;
          }
          uint32_t ms = (get_clock_ticks() - resume_speed_ms);
          float increment = (ms / 1000.0) * 45.0;
          velocidad = 10 + increment;
        } else if (velocidad != velocidadIdeal) {
          velocidad = velocidadIdeal;
        }
        float velI = velocidad - correccion_velocidad;
        float velD = velocidad + correccion_velocidad;

        if (velD < 10) {
          velI += 10 - velD;
          velD = 10;
        } else if (velD > 100) {
          velD = 100;
        }
        if (velI < 10) {
          velD += 10 - velI;
          velI = 10;
        } else if (velI > 100) {
          velI = 100;
        }
        set_motors_speed(velD, velI);
        set_fan_speed(velocidadVentiladorIdeal);
      } else {
        velocidad = velocidadIdeal;
        set_motors_speed(0, 0);
      }
    }
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
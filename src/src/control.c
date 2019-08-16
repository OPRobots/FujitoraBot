#include <control.h>

static int32_t velocidad = 0;
static bool competicionIniciada = false;
volatile static int32_t correccion_velocidad = 0;
volatile static int32_t error_anterior = 0;
static bool vuelta_iniciada = false;
static bool left_mark = false;
static bool right_mark = false;

static int32_t calc_pid_correction(int32_t posicion) {
  double p = 0;
  double i = 0;
  double d = 0;
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

void pid_timer_custom_isr() {
  calc_sensor_line_position();
  correccion_velocidad = calc_pid_correction(get_sensor_line_position());
  check_side_marks();

  if (!right_mark && is_right_mark()) {
    if (!vuelta_iniciada) {
      vuelta_iniciada = true;
      set_status_led(true);
    } else {
      vuelta_iniciada = false;
      all_leds_clear();
      set_competicion_iniciada(false);
      pause_pid_timer();
      pause_speed_timer();
    }
  }

  if (is_left_mark() || is_right_mark()) {
    set_neon_fade(1024);
  } else {
    set_neon_fade(0);
  }

  left_mark = is_left_mark();
  right_mark = is_right_mark();
}

int32_t get_speed_correction() {
  return correccion_velocidad;
}

void speed_timer_custom_isr() {
  if (velocidad > 0) {
    int32_t velI = velocidad - correccion_velocidad;
    int32_t velD = velocidad + correccion_velocidad;
    velD = constrain(velD, 15, 100);
    velI = constrain(velI, 15, 100);
    set_motors_speed(velD, velI);
  } else {
    set_motors_speed(0, 0);
  }
}

void set_speed(int32_t v) {
  velocidad = v;
}

void resume_pid_timer() {
  timer_enable_irq(TIM5, TIM_DIER_CC1IE);
}

void pause_pid_timer() {
  timer_disable_irq(TIM5, TIM_DIER_CC1IE);
  correccion_velocidad = 0;
}

void resume_speed_timer() {
  timer_enable_irq(TIM2, TIM_DIER_CC1IE);
}

void pause_speed_timer() {
  timer_disable_irq(TIM2, TIM_DIER_CC1IE);
  set_motors_speed(0, 0);
  all_leds_clear();
}

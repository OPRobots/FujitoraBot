#include "robotracer.h"

static float acceleration_mss = MAX_ACCEL_MS2;
static float deceleration_mss = MAX_BREAK_MS2;
static float turn_speed = 1.25f;
static float straight_speed = 4.0f;
static float turn_acceleration_mss = 0.8038f;

static bool left_mark = false;
static bool right_mark = false;

bool map_iniciado = false;
bool map_realizado = false;
bool vuelta_finalizada = false;

bool run_iniciado = false;
bool run_realizado = false;
uint16_t map_sector_actual = 0;
uint16_t run_sector_actual = 0;

uint32_t last_left_ticks = 0;
uint32_t last_right_ticks = 0;

uint32_t last_check_sector_left_ticks = 0;
uint32_t last_check_sector_right_ticks = 0;

#define MAX_CURRENT_SECTOR_SAMPLES 1000
uint8_t current_sector_radius_samples = 0;
float current_sector_radius[MAX_CURRENT_SECTOR_SAMPLES];
float current_sector_radius_filter = 0;

uint32_t pista_sectores[1000];
uint32_t pista_sectores_run[1000];
float sectores_tipos[1000];

static void reset_encoders(void) {
  last_left_ticks = get_encoder_left_total_ticks();
  last_right_ticks = get_encoder_right_total_ticks();
  last_check_sector_left_ticks = get_encoder_left_total_ticks();
  last_check_sector_right_ticks = get_encoder_right_total_ticks();
}

static void init_map(void) {
  printf("Mapeo iniciado\n");
  set_status_led(true);
  reset_encoders();
  map_iniciado = true;
  set_fans_speed(35, 35);
}

static void end_map(void) {
  printf("Mapeo finalizado\n");
  set_status_led(false);
  map_realizado = true;

  // for (int i = 0; i < vector_size(pista_sectores); i++) {
  for (int i = 0; i < map_sector_actual; i++) {
    printf("%ld (r=%.2fcm - l=%.2fm)\n", pista_sectores[i], sectores_tipos[i], pista_sectores[i] * get_micrometers_per_tick() / MICROMETERS_PER_METER);
  }

  reset_encoders();
}

static void robotracer_map_last_sector(void) {
  uint32_t avg_ticks_sector = (max_likelihood_counter_diff(get_encoder_left_total_ticks(), last_left_ticks) + max_likelihood_counter_diff(get_encoder_right_total_ticks(), last_right_ticks)) / 2;

  float left_distance_meters = max_likelihood_counter_diff(get_encoder_left_total_ticks(), last_left_ticks) * get_micrometers_per_tick() / MICROMETERS_PER_METER;
  float right_distance_meters = max_likelihood_counter_diff(get_encoder_right_total_ticks(), last_right_ticks) * get_micrometers_per_tick() / MICROMETERS_PER_METER;
  float radius = (WHEELS_SEPARATION / 2.0f) * (left_distance_meters + right_distance_meters) / (left_distance_meters - right_distance_meters) * 100;
  if (isnan(radius) || isinf(radius) || abs(radius) > 100) {
    radius = 0;
  }

  float avg_distance_meters = avg_ticks_sector * get_micrometers_per_tick() / MICROMETERS_PER_METER;
  if (avg_distance_meters > 0.1 || (radius > 0.0f && radius < 100.0f)) {

    // vector_add(&sectores_tipos, float) = radius;
    // vector_add(&pista_sectores, uint32_t) = avg_ticks_sector;
    sectores_tipos[map_sector_actual] = radius;
    pista_sectores[map_sector_actual] = avg_ticks_sector;
    map_sector_actual++;

    reset_encoders();
    current_sector_radius_samples = 0;
  }
}

static float calc_run_max_turn_speed(float radius) {
  if (radius == 0) {
    return straight_speed;
  }
  // u = 2.74?
  // float a_max = 0.8038f;
  float max_turn_speed = 3.6f * sqrt(turn_acceleration_mss * abs(radius) / 100.0f);

  // if (ticks_lenght <= 0.35 * MICROMETERS_PER_METER / get_micrometers_per_tick() && abs(max_turn_speed - get_encoder_avg_speed()) > 2) {
  //   max_turn_speed = max_turn_speed / 2;
  // }
  // printf("%.4f=3.6*sqrt(%.4f*%.4f)\n", max_turn_speed, a_max, radius / 100.0f);
  if (max_turn_speed < turn_speed || radius < 20) {
    max_turn_speed = turn_speed;
  }
  return max_turn_speed;
}

static void set_ideal_speed_sector_actual(void) {
  if (abs(sectores_tipos[run_sector_actual]) < 5) {
    set_ideal_motors_ms_speed(straight_speed);
    set_fans_speed(40, 40);
    set_status_led(false);
    set_RGB_color(0, 255, 0);
  } else {
    // set_ideal_motors_ms_speed(turn_speed);
    set_ideal_motors_ms_speed(calc_run_max_turn_speed(sectores_tipos[run_sector_actual]));
    set_fans_speed(75, 75);
    set_status_led(true);
    set_RGB_color(0, 0, 255);
  }
}

static void init_run(void) {
  run_iniciado = true;
  run_realizado = false;
  run_sector_actual = 0;
  set_fans_speed(35, 35);
  reset_encoders();
  set_ideal_speed_sector_actual();
}

static void end_run(void) {
  run_realizado = true;
  reset_encoders();
}

static void run_next_sector(void) {
  uint32_t avg_ticks_sector = (max_likelihood_counter_diff(get_encoder_left_total_ticks(), last_left_ticks) + max_likelihood_counter_diff(get_encoder_right_total_ticks(), last_right_ticks)) / 2;
  if (avg_ticks_sector > pista_sectores_run[run_sector_actual] * 0.80f) {
    reset_encoders();
    run_sector_actual++;
    set_ideal_speed_sector_actual();
  }
}

static void check_next_sector_radius(void) {
  if (map_realizado && run_iniciado) {

    float avg_speed = get_encoder_avg_speed();

    uint16_t sector_check_radius = run_sector_actual + 1;
    while (sectores_tipos[sector_check_radius] == 0 && sector_check_radius < map_sector_actual) {
      sector_check_radius++;
    }
    float next_turn_radius = sectores_tipos[sector_check_radius];

    float next_turn_speed = (sector_check_radius < map_sector_actual) ? calc_run_max_turn_speed(next_turn_radius) : turn_speed;
    if (avg_speed > next_turn_speed) {
      float time_to_stop = (next_turn_speed - get_encoder_avg_speed()) / -deceleration_mss;
      float meters_to_stop = (get_encoder_avg_speed() * time_to_stop) - (0.5 * deceleration_mss * (time_to_stop * time_to_stop));
      meters_to_stop += (avg_speed - next_turn_speed) * 0.125; // Añade una distancia de seguridad a la frenada
      uint32_t ticks_to_stop = (meters_to_stop)*MICROMETERS_PER_METER / get_micrometers_per_tick();
      int32_t avg_ticks_sector = (max_likelihood_counter_diff(get_encoder_left_total_ticks(), last_left_ticks) + max_likelihood_counter_diff(get_encoder_right_total_ticks(), last_right_ticks)) / 2;

      uint32_t ticks_recto = (pista_sectores_run[run_sector_actual] - avg_ticks_sector);
      uint16_t sector_check = run_sector_actual + 1;
      while (sector_check < map_sector_actual && sectores_tipos[sector_check] == 0) {
        ticks_recto += pista_sectores_run[sector_check];
        sector_check++;
      }
      if (ticks_recto <= ticks_to_stop) {
        set_ideal_motors_ms_speed(next_turn_speed);
        set_RGB_color(255, 0, 0);
        set_fans_speed(50, 50);
        set_status_led(true);
      }
    }
  }
}

/* void check_next_sector_radius(void) {
  if (map_realizado && run_iniciado) {
    if (sectores_tipos[run_sector_actual] == 0 || sectores_tfipos[run_sector_actual] >= 80) {
      float avg_speed = get_encoder_avg_speed();
      if (avg_speed > turn_speed) {
        float time_to_stop = (turn_speed - get_encoder_avg_speed()) / -deceleration_mss;
        float meters_to_stop = (get_encoder_avg_speed() * time_to_stop) - (0.5 * deceleration_mss * (time_to_stop * time_to_stop));
        meters_to_stop += (avg_speed - turn_speed) * 0.09; // Añade una distancia de seguridad a la frenada
        int32_t ticks_to_stop = (meters_to_stop)*MICROMETERS_PER_METER / get_micrometers_per_tick();
        int32_t avg_ticks_sector = (max_likelihood_counter_diff(get_encoder_left_total_ticks(), last_left_ticks) + max_likelihood_counter_diff(get_encoder_right_total_ticks(), last_right_ticks)) / 2;

        uint32_t ticks_recto = (pista_sectores_run[run_sector_actual] - avg_ticks_sector);
        uint16_t sector_check = run_sector_actual + 1;
        while (sector_check < map_sector_actual && (sectores_tipos[sector_check] == 0 || sectores_tipos[sector_check] >= 80)) {
          ticks_recto += pista_sectores_run[sector_check];
          sector_check++;
        }
        if (ticks_recto <= ticks_to_stop) {
          set_ideal_motors_ms_speed(turn_speed);
          set_RGB_color(255, 0, 0);
          set_fans_speed(50, 50);
          set_status_led(true);
        }
      }
    }
  }
} */

static void robotracer_left_mark(void) {
  // return;
  if (map_realizado) {
    run_next_sector();
  } else {
    robotracer_map_last_sector();
  }
}

static void robotracer_right_mark(void) {
  // return;
  if (!map_realizado) {
    if (map_iniciado) {
      robotracer_map_last_sector();
      end_map();
      vuelta_finalizada = true;
    } else {
      init_map();
    }
  } else if (!run_realizado) {
    if (run_iniciado) {
      end_run();
      vuelta_finalizada = true;
    } else {
      init_run();
    }
  }
}

static bool robotracer_can_stop(void) {
  // return false;
  if (((map_iniciado && map_realizado && !run_iniciado && !run_realizado) || (map_iniciado && map_realizado && run_iniciado && run_realizado)) && vuelta_finalizada) {
    uint32_t avg_ticks_after_finish = (max_likelihood_counter_diff(get_encoder_left_total_ticks(), last_left_ticks) + max_likelihood_counter_diff(get_encoder_right_total_ticks(), last_right_ticks)) / 2;
    float meters_after_finish = avg_ticks_after_finish * get_micrometers_per_tick() / MICROMETERS_PER_METER;
    if (meters_after_finish > 0.50) {
      vuelta_finalizada = false;
      set_fans_speed(0, 0);
      return true;
    } else {
      return false;
    }
  } else {
    return false;
  }
}

static void robotracer_check_sector_ends_before_mark(void) {
  if (run_iniciado && !run_realizado) {
    uint32_t avg_ticks_sector = (max_likelihood_counter_diff(get_encoder_left_total_ticks(), last_left_ticks) + max_likelihood_counter_diff(get_encoder_right_total_ticks(), last_right_ticks)) / 2;

    uint32_t ticks_max = (uint32_t)(0.15 * MICROMETERS_PER_METER / get_micrometers_per_tick());
    if (avg_ticks_sector >= (pista_sectores_run[run_sector_actual] + ticks_max)) {
      reset_encoders();
      run_sector_actual++;
      if (pista_sectores_run[run_sector_actual] - ticks_max > 0) {
        pista_sectores_run[run_sector_actual] -= ticks_max;
      } else {
        pista_sectores_run[run_sector_actual] = 0;
        // ¿Debería de pasar al siguiente sector?
      }
      set_ideal_speed_sector_actual();
    }
  }
}

void robotracer_loop_flow(void) {
  check_side_marks();
  // check_sector_radius();
  check_next_sector_radius();
  if (!right_mark && is_right_mark()) {
    robotracer_right_mark();
  }
  if (robotracer_can_stop()) {
    set_competicion_iniciada(false);
    pause_pid_speed_timer();
    set_neon_fade(0);
    set_RGB_color(0, 0, 0);
    all_leds_clear();
  }

  robotracer_check_sector_ends_before_mark();

  if (left_mark && !is_left_mark()) {
    robotracer_left_mark();
    set_neon_fade(50);
  } else {
    set_neon_fade(0);
  }

  left_mark = is_left_mark();
  right_mark = is_right_mark();
}

void robotracer_restart(void) {
  if (run_iniciado || map_realizado) {
    run_sector_actual = 0;
    run_iniciado = false;
    run_realizado = false;
    vuelta_finalizada = false;
  }
  if (map_realizado) {
    for (uint16_t sector = 0; sector < map_sector_actual; sector++) {
      pista_sectores_run[sector] = pista_sectores[sector];
    }
  }
}

void robotracer_set_turn_speed(float ms) {
  turn_speed = ms;
}

void robotracer_set_straight_speed(float ms) {
  straight_speed = ms;
}

void robotracer_set_acceleration_mss(float mss) {
  acceleration_mss = mss;
}

void robotracer_set_deceleration_mss(float mss) {
  deceleration_mss = mss;
}

void robotracer_set_turn_acceleration_mss(float mss) {
  turn_acceleration_mss = mss;
}

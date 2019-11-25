#include <robotracer.h>

static float TURN_SPEED = 1.25f;
static float STRAIGHT_SPEED = 4.0f;

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
float sectores_tipos[1000];

static void reset_encoders() {
  last_left_ticks = get_encoder_left_total_ticks();
  last_right_ticks = get_encoder_right_total_ticks();
  last_check_sector_left_ticks = get_encoder_left_total_ticks();
  last_check_sector_right_ticks = get_encoder_right_total_ticks();
}

static void init_map() {
  printf("Mapeo iniciado\n");
  set_status_led(true);
  reset_encoders();
  map_iniciado = true;
  // pista_sectores = vector_create();
  // sectores_tipos = vector_create();
}

static void end_map() {
  printf("Mapeo finalizado\n");
  set_status_led(false);
  map_realizado = true;

  // for (int i = 0; i < vector_size(pista_sectores); i++) {
  for (int i = 0; i < map_sector_actual; i++) {
    printf("%ld (r=%.2fcm - l=%.2fm)\n", pista_sectores[i], sectores_tipos[i], pista_sectores[i] * MICROMETERS_PER_TICK / MICROMETERS_PER_METER);
  }

  reset_encoders();
}

static void robotracer_last_sector() {
  uint32_t avg_ticks_sector = (max_likelihood_counter_diff(get_encoder_left_total_ticks(), last_left_ticks) + max_likelihood_counter_diff(get_encoder_right_total_ticks(), last_right_ticks)) / 2;

  float left_distance_meters = max_likelihood_counter_diff(get_encoder_left_total_ticks(), last_left_ticks) * MICROMETERS_PER_TICK / MICROMETERS_PER_METER;
  float right_distance_meters = max_likelihood_counter_diff(get_encoder_right_total_ticks(), last_right_ticks) * MICROMETERS_PER_TICK / MICROMETERS_PER_METER;
  float radius = (((WHEELS_SEPARATION * left_distance_meters) / (left_distance_meters - right_distance_meters)) - (WHEELS_SEPARATION / 2.0f)) * 100;
  if (isnan(radius) || isinf(radius) || abs(radius) > 40) {
    radius = 0;
  }

  float avg_distance_meters = avg_ticks_sector * MICROMETERS_PER_TICK / MICROMETERS_PER_METER;
  if (avg_distance_meters > 0.1 || (radius > 0.0f && radius < 40.0f)) {

    // vector_add(&sectores_tipos, float) = radius;
    // vector_add(&pista_sectores, uint32_t) = avg_ticks_sector;
    sectores_tipos[map_sector_actual] = radius;
    pista_sectores[map_sector_actual] = avg_ticks_sector;
    map_sector_actual++;

    reset_encoders();
    current_sector_radius_samples = 0;
  }
}

static void set_ideal_speed_sector_actual() {
  if (abs(sectores_tipos[run_sector_actual]) < 5) {
    set_ideal_motors_ms_speed(STRAIGHT_SPEED);
    set_RGB_color(0, 255, 0);
  } else {
    set_ideal_motors_ms_speed(TURN_SPEED);
    set_RGB_color(0, 0, 255);
  }
}

static void init_run() {
  run_iniciado = true;
  run_realizado = false;
  run_sector_actual = 0;
  reset_encoders();
  set_ideal_speed_sector_actual();
}

static void end_run() {
  run_realizado = true;
  reset_encoders();
}

static void run_next_sector() {
  uint32_t avg_ticks_sector = (max_likelihood_counter_diff(get_encoder_left_total_ticks(), last_left_ticks) + max_likelihood_counter_diff(get_encoder_right_total_ticks(), last_right_ticks)) / 2;
  if (avg_ticks_sector > pista_sectores[run_sector_actual] * 0.80f) {
    reset_encoders();
    run_sector_actual++;
    set_ideal_speed_sector_actual();
  }
}

void check_next_sector_radius() {
  if (map_realizado && run_iniciado) {
    if (sectores_tipos[run_sector_actual] == 0 || sectores_tipos[run_sector_actual] >= 80) {
      float avg_speed = get_encoder_avg_speed();
      if (avg_speed > TURN_SPEED) {
        float time_to_stop = (TURN_SPEED - get_encoder_avg_speed()) / -MAX_BREAK_MS2;
        float meters_to_stop = (get_encoder_avg_speed() * time_to_stop) - (0.5 * MAX_BREAK_MS2 * (time_to_stop * time_to_stop));
        meters_to_stop += (avg_speed - TURN_SPEED) * 0.09; // Añade una distancia de seguridad a la frenada
        int32_t ticks_to_stop = (meters_to_stop) * MICROMETERS_PER_METER / MICROMETERS_PER_TICK;
        int32_t avg_ticks_sector = (max_likelihood_counter_diff(get_encoder_left_total_ticks(), last_left_ticks) + max_likelihood_counter_diff(get_encoder_right_total_ticks(), last_right_ticks)) / 2;

        uint32_t ticks_recto = (pista_sectores[run_sector_actual] - avg_ticks_sector);
        uint16_t sector_check = run_sector_actual + 1;
        while (sector_check < map_sector_actual && (sectores_tipos[sector_check] == 0 || sectores_tipos[sector_check] >= 80)) {
          ticks_recto += pista_sectores[sector_check];
          sector_check++;
        }
        if (ticks_recto <= ticks_to_stop) {
            set_ideal_motors_ms_speed(TURN_SPEED);
            set_RGB_color(255, 0, 0);
        }
      }
    }
  }
}

void robotracer_left_mark() {
  // return;
  if (map_realizado) {
    run_next_sector();
  } else {
    robotracer_last_sector();
  }
}

void robotracer_right_mark() {
  // return;
  if (!map_realizado) {
    if (map_iniciado) {
      robotracer_last_sector();
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

bool robotracer_can_stop() {
  // return false;
  if (((map_iniciado && map_realizado && !run_iniciado && !run_realizado) || (map_iniciado && map_realizado && run_iniciado && run_realizado)) && vuelta_finalizada) {
    uint32_t avg_ticks_after_finish = (max_likelihood_counter_diff(get_encoder_left_total_ticks(), last_left_ticks) + max_likelihood_counter_diff(get_encoder_right_total_ticks(), last_right_ticks)) / 2;
    float meters_after_finish = avg_ticks_after_finish * MICROMETERS_PER_TICK / MICROMETERS_PER_METER;
    if (meters_after_finish > 0.20) {
      vuelta_finalizada = false;
      return true;
    } else {
      return false;
    }
  } else {
    return false;
  }
}

void robotracer_check_sector_ends_before_mark() {
  if (run_iniciado && !run_realizado) {
    uint32_t avg_ticks_sector = (max_likelihood_counter_diff(get_encoder_left_total_ticks(), last_left_ticks) + max_likelihood_counter_diff(get_encoder_right_total_ticks(), last_right_ticks)) / 2;
    if (avg_ticks_sector >= pista_sectores[run_sector_actual] * 1.20f) {
      run_sector_actual++;
      reset_encoders();
      set_ideal_speed_sector_actual();
    }
  }
}

void robotracer_restart() {
  if (run_iniciado || map_realizado) {
    run_sector_actual = 0;
    run_iniciado = false;
    run_realizado = false;
  }
}
#include <robotracer.h>

bool map_iniciado = false;
bool map_realizado = false;
bool vuelta_finalizada = false;

bool run_iniciado = false;
bool run_realizado = false;
uint16_t run_sector_actual = 0;

uint32_t last_left_ticks = 0;
uint32_t last_right_ticks = 0;

uint32_t last_check_sector_left_ticks = 0;
uint32_t last_check_sector_right_ticks = 0;

#define MAX_CURRENT_SECTOR_SAMPLES 1000
uint8_t current_sector_radius_samples = 0;
float current_sector_radius[MAX_CURRENT_SECTOR_SAMPLES];
float current_sector_radius_filter = 0;

uint32_t *pista_sectores;
float *sectores_tipos;

static void reset_encoders() {
  last_left_ticks = get_encoder_left_total_ticks();
  last_right_ticks = get_encoder_right_total_ticks();
  last_check_sector_left_ticks = get_encoder_left_total_ticks();
  last_check_sector_right_ticks = get_encoder_right_total_ticks();
}

static void init_map() {
  printf("Mapeo iniciado\n");
  reset_encoders();
  map_iniciado = true;
  pista_sectores = vector_create();
  sectores_tipos = vector_create();
}

static void end_map() {
  printf("Mapeo finalizado\n");
  map_realizado = true;

  for (int i = 0; i < vector_size(pista_sectores); i++) {
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

    vector_add(&sectores_tipos, float) = radius;
    vector_add(&pista_sectores, uint32_t) = avg_ticks_sector;

    reset_encoders();
    current_sector_radius_samples = 0;
  }
}

static void set_ideal_speed_sector_actual() {
  if (abs(sectores_tipos[run_sector_actual]) < 5) {
    set_ideal_motors_ms_speed(2.5f);
    set_status_led(true);
  } else {
    set_ideal_motors_ms_speed(1.0);
    set_status_led(false);
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

/* void check_sector_radius() {
  if (map_iniciado && !map_realizado) {

    float left_distance_meters = max_likelihood_counter_diff(get_encoder_left_total_ticks(), last_check_sector_left_ticks) * MICROMETERS_PER_TICK / MICROMETERS_PER_METER;
    float right_distance_meters = max_likelihood_counter_diff(get_encoder_right_total_ticks(), last_check_sector_right_ticks) * MICROMETERS_PER_TICK / MICROMETERS_PER_METER;

    last_check_sector_left_ticks = get_encoder_left_total_ticks();
    last_check_sector_right_ticks = get_encoder_right_total_ticks();

    float radius = ((WHEELS_SEPARATION * left_distance_meters) / (left_distance_meters - right_distance_meters)) - (WHEELS_SEPARATION / 2.0f);

    if (isnan(radius) || isinf(radius) || abs(get_encoder_angular_speed()) < 2.5) {
      radius = 0;
    } else {
      radius = radius * 100;
    }
    if (current_sector_radius_samples < MAX_CURRENT_SECTOR_SAMPLES) {
      current_sector_radius[current_sector_radius_samples] = radius;
      current_sector_radius_samples++;
    } else {
      // for (uint8_t sector_sample = MAX_CURRENT_SECTOR_SAMPLES - 1; sector_sample > 0; sector_sample--) {
      //   current_sector_radius[sector_sample] = current_sector_radius[sector_sample - 1];
      // }
      // current_sector_radius[0] = radius;
    }
    float sum_radius = 0;
    for (uint8_t sector_sample = current_sector_radius_samples / 4; sector_sample < current_sector_radius_samples * 3 / 4; sector_sample++) {
      sum_radius += current_sector_radius[sector_sample];
    }
    current_sector_radius_filter = sum_radius / (current_sector_radius_samples / 2);
    if (abs(current_sector_radius_filter) > 5.0f && abs(current_sector_radius_filter) < 80.0f) {
      set_status_led(true);
    } else {
      set_status_led(false);
      current_sector_radius_filter = 0;
    }
    // printf("%.4f - %.4f\n", current_sector_radius_filter, radius);
  }
}
 */
/* void check_sector_radius() {
  if (map_iniciado && !map_realizado) {
    float radius = (1.0f / get_encoder_angular_speed()) * 100; // radius;
    if (isnan(radius) || isinf(radius) || abs(get_encoder_angular_speed()) < 3) {
      radius = 0;
    }
    if (current_sector_radius_samples < MAX_CURRENT_SECTOR_SAMPLES) {
      current_sector_radius[current_sector_radius_samples] = radius;
      current_sector_radius_samples++;
    } else {
      for (uint8_t sector_sample = MAX_CURRENT_SECTOR_SAMPLES - 1; sector_sample > 0; sector_sample--) {
        current_sector_radius[sector_sample] = current_sector_radius[sector_sample - 1];
      }
      current_sector_radius[0] = radius;
    }
    float sum_radius = 0;
    for (uint8_t sector_sample = 0; sector_sample < current_sector_radius_samples; sector_sample++) {
      sum_radius += current_sector_radius[sector_sample];
    }
    current_sector_radius_filter = sum_radius / current_sector_radius_samples;
    if (abs(current_sector_radius_filter) > 5.0f) {
      set_status_led(true);
    } else {
      set_status_led(false);
      current_sector_radius_filter = 0;
    }
  }
} */

void check_next_sector_radius() {
  if (map_realizado && run_iniciado) {
    bool slow_down = false;
    if (vector_size(sectores_tipos) > run_sector_actual + 1) {
      if (abs(sectores_tipos[run_sector_actual + 1]) >= 5) {
        slow_down = true;
      }
    } else if (vector_size(sectores_tipos) == run_sector_actual + 1) {
      slow_down = true;
    }
    if (slow_down) {
      float time_to_stop = abs((1.0f - get_encoder_avg_speed()) / MAX_ACCEL_MS2);
      float meters_to_stop = get_encoder_avg_speed() * time_to_stop - 0.5 * MAX_ACCEL_MS2 * (time_to_stop * time_to_stop);
      int32_t ticks_to_stop = (meters_to_stop + 0.4f) * MICROMETERS_PER_METER / MICROMETERS_PER_TICK;
      int32_t avg_ticks_sector = (max_likelihood_counter_diff(get_encoder_left_total_ticks(), last_left_ticks) + max_likelihood_counter_diff(get_encoder_right_total_ticks(), last_right_ticks)) / 2;
      if ((pista_sectores[run_sector_actual] - avg_ticks_sector) <= ticks_to_stop) {
        set_ideal_motors_ms_speed(1.0);
        warning_status_led(50);
      }
    }
  }
}

void robotracer_left_mark() {
  if (map_realizado) {
    run_next_sector();
  } else {
    robotracer_last_sector();
  }
}

void robotracer_right_mark() {
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
    if (avg_ticks_sector >= pista_sectores[run_sector_actual] * 1.25f) {
      run_sector_actual++;
      reset_encoders();
      set_ideal_speed_sector_actual();
    }
  }
}
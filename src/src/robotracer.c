#include <robotracer.h>

bool mapeo_iniciado = false;
bool mapeo_realizado = false;

uint32_t last_left_ticks = 0;
uint32_t last_right_ticks = 0;

#define MAX_CURRENT_SECTOR_SAMPLES 30
uint8_t current_sector_radius_samples = 0;
float current_sector_radius[MAX_CURRENT_SECTOR_SAMPLES];

uint32_t *pista_sectores;
uint8_t *sectores_tipos;

static void reset_encoders() {
  last_left_ticks = get_encoder_left_total_ticks();
  last_right_ticks = get_encoder_right_total_ticks();
}

static void init_map() {
  printf("Mapeo iniciado\n");
  reset_encoders();
  mapeo_iniciado = true;
  pista_sectores = vector_create();
}

static void end_map() {
  printf("Mapeo finalizado\n");
  mapeo_realizado = true;
  reset_encoders();
}

static void save_last_sector() {
  uint32_t avg_ticks_sector = (get_encoder_left_total_ticks() + get_encoder_right_total_ticks()) / 2 - (last_left_ticks + last_right_ticks) / 2;
  //   printf("Sector guardado (%ld + %ld)/2 - (%ld + %ld)/2 = %ld\n", get_encoder_left_total_ticks(), get_encoder_right_total_ticks(), last_left_ticks, last_right_ticks, avg_ticks_sector);
  vector_add(&pista_sectores, uint32_t) = avg_ticks_sector;
  reset_encoders();
  current_sector_radius_samples = 0;

  for (int i = 0; i < vector_size(pista_sectores); i++) {
    printf("%ld\t", pista_sectores[i]);
  }
  printf("\n");
}

void check_sector_radius() {
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
  float radius_filter = sum_radius / current_sector_radius_samples;
  if (abs(radius_filter) > 5.0f) {
    set_status_led(true);
  } else {
    set_status_led(false);
    radius_filter = 0;
  }
  printf("35\t-35\t%.4f\n", /* get_encoder_angular_speed(), */ radius_filter);
}

void map_left_mark() {
  if (mapeo_realizado) {
    // TODO: Resolución
  } else {
    save_last_sector();
  }
}

void map_right_mark() {
  if (mapeo_iniciado) {
    // TODO: consolidar_sector
    end_map();
    // TODO: detener robot(?)
  } else {
    init_map();
  }
}
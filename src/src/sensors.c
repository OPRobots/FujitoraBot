#include <sensors.h>

#define NUM_SENSORS 16
#define NUM_SENSORS_LINE 12

static uint8_t sensores[NUM_SENSORS] = {ADC_CHANNEL0, ADC_CHANNEL1, ADC_CHANNEL2, ADC_CHANNEL3, ADC_CHANNEL4, ADC_CHANNEL5, ADC_CHANNEL6, ADC_CHANNEL7, ADC_CHANNEL14, ADC_CHANNEL15, ADC_CHANNEL8, ADC_CHANNEL9, ADC_CHANNEL10, ADC_CHANNEL11, ADC_CHANNEL12, ADC_CHANNEL13};
static volatile uint16_t sensores_raw[NUM_SENSORS];
static uint16_t sensores_max[NUM_SENSORS];
static uint16_t sensores_min[NUM_SENSORS];
static uint16_t sensores_umb[NUM_SENSORS];
static int32_t line_position;
static uint32_t ultimaLinea = 0;
static bool left_mark = false;
static bool right_mark = false;

uint8_t *get_sensors() {
  return sensores;
}

uint8_t get_sensors_num() {
  return NUM_SENSORS;
}

uint8_t get_sensors_line_num() {
  return NUM_SENSORS_LINE;
}

volatile uint16_t *get_sensors_raw() {
  return sensores_raw;
}

uint16_t get_sensor_raw(uint8_t pos) {
  if (pos < NUM_SENSORS) {
    return sensores_raw[pos];
  } else {
    return 0;
  }
}

uint16_t get_sensor_calibrated(uint8_t pos) {
  if (pos < NUM_SENSORS) {
    uint16_t sensor_calibrado = get_sensor_raw(pos);
    if (sensor_calibrado >= sensores_umb[pos]) {
      sensor_calibrado = LECTURA_MAXIMO_SENSORES_LINEA;
    } else {
      sensor_calibrado = LECTURA_MINIMO_SENSORES_LINEA;
    }
    if (get_config_track() == CONFIG_TRACK_ROBOTRACER) {
      return LECTURA_MAXIMO_SENSORES_LINEA - sensor_calibrado;
    } else {
      return sensor_calibrado;
    }
  } else {
    return 0;
  }
}

void calibrate_sensors() {
  bool auto_move = get_config_run() == CONFIG_RUN_RACE; // En modo carrera, se calibra automáticamente por defecto.
  bool pushFlag = false;
  while (!get_start_btn()) {
    set_neon_heartbeat();
    if (get_menu_mode_btn()) {
      if (!pushFlag) {
        auto_move = !auto_move;
        pushFlag = true;
      }
    } else {
      pushFlag = false;
    }
    set_status_led(auto_move);
  }
  while (get_start_btn()) {
    set_neon_heartbeat();
  }
  while (auto_move && !is_esc_inited()) {
  }
  set_neon_fade(0);
  set_status_led(false);
  delay(1000);

  // Resetear los valores máximos, mínimos y umbrales
  for (uint8_t sensor = 0; sensor < NUM_SENSORS; sensor++) {
    sensores_max[sensor] = LECTURA_MINIMO_SENSORES_LINEA;
    sensores_min[sensor] = LECTURA_MAXIMO_SENSORES_LINEA;
    sensores_umb[sensor] = LECTURA_MINIMO_SENSORES_LINEA;
  }

  uint32_t ms_inicio = get_clock_ticks();
  while (ms_inicio + MS_CALIBRACION_LINEA >= get_clock_ticks()) {
    for (int sensor = 0; sensor < NUM_SENSORS; sensor++) {
      if (sensores_raw[sensor] < sensores_min[sensor]) {
        sensores_min[sensor] = sensores_raw[sensor];
      }
      if (sensores_raw[sensor] > sensores_max[sensor]) {
        sensores_max[sensor] = sensores_raw[sensor];
      }
      set_RGB_rainbow();
      if (auto_move) {
        set_motors_speed(20, -20);
      }
    }
  }
  if (auto_move) {
    set_motors_speed(0, 0);
  }

  bool calibrationOK = true;
  bool marksOK = true;
  for (int sensor = 0; sensor < NUM_SENSORS; sensor++) {
    if (abs(sensores_max[sensor] - sensores_min[sensor]) < 1000) {
      if (sensor >= NUM_SENSORS_LINE) {
        marksOK = false;
      } else {
        calibrationOK = false;
      }
    }
    sensores_umb[sensor] = (sensores_max[sensor] + sensores_min[sensor]) / 2.;
  }

  while (!get_start_btn()) {
    if (calibrationOK && marksOK) {
      set_RGB_color(0, 100, 0);
    } else if (!calibrationOK) {
      set_RGB_color(100, 0, 0);
    } else if (!marksOK) {
      set_RGB_color(100, 20, 0);
    }
  }
  while (get_start_btn()) {
    if (calibrationOK) {
      set_RGB_color(0, 100, 0);
    } else {
      set_RGB_color(100, 0, 0);
    }
  }
  set_RGB_color(0, 0, 0);
  delay(250);
}

int32_t get_sensor_line_position() {
  return line_position;
}

void calc_sensor_line_position() {
  uint32_t suma_sensores_ponderados = 0;
  uint32_t suma_sensores = 0;
  uint8_t sensores_detectando = 0;

  for (uint8_t sensor = 0; sensor < NUM_SENSORS_LINE; sensor++) {
    if (get_sensor_calibrated(sensor) >= sensores_umb[sensor]) {
      sensores_detectando++;
    }
    suma_sensores_ponderados += (sensor + 1) * get_sensor_calibrated(sensor) * 1000;
    suma_sensores += get_sensor_calibrated(sensor);
  }

  if (sensores_detectando > 0 && sensores_detectando < NUM_SENSORS_LINE) {
    ultimaLinea = get_clock_ticks();
  } else if (get_clock_ticks() > (ultimaLinea + get_offtrack_time())) {
    set_competicion_iniciada(false);
    pause_pid_timer();
    pause_speed_timer();
  }

  int32_t posicion;
  if (sensores_detectando > 0) {
    posicion = ((suma_sensores_ponderados / suma_sensores) - ((NUM_SENSORS_LINE + 1) * 1000) / 2);
  } else {
    posicion = (line_position >= 0) ? ((1000 * (NUM_SENSORS_LINE + 1)) / 2) : -((1000 * (NUM_SENSORS_LINE + 1)) / 2);
  }
  posicion = map(posicion, -6500, 6500, -1000, 1000);

  line_position = posicion;
}

void check_side_marks() {
  bool side_marks[4] = {false, false, false, false};
  for (uint8_t sensor = NUM_SENSORS_LINE; sensor < NUM_SENSORS; sensor++) {
    side_marks[sensor - NUM_SENSORS_LINE] = get_sensor_calibrated(sensor) >= sensores_umb[sensor];
  }
  bool left = side_marks[2] || side_marks[3];
  bool right = side_marks[0] || side_marks[1];

  left_mark = left && !(left && right);
  right_mark = right && !(left && right);
}

bool is_left_mark() {
  return left_mark;
}

bool is_right_mark() {
  return right_mark;
}
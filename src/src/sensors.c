#include "sensors.h"

// ADC de derecha a izquierda
static uint8_t sensores_linefollower[NUM_SENSORES_LINEFOLLOWER] = {ADC_CHANNEL11, ADC_CHANNEL10, ADC_CHANNEL0, ADC_CHANNEL1, ADC_CHANNEL2, ADC_CHANNEL3, ADC_CHANNEL4, ADC_CHANNEL5, ADC_CHANNEL6, ADC_CHANNEL7, ADC_CHANNEL14, ADC_CHANNEL15, ADC_CHANNEL8, ADC_CHANNEL9, ADC_CHANNEL12, ADC_CHANNEL13};

static uint8_t sensores_robotracer[NUM_SENSORES_ROBOTRACER + NUM_SENSORES_ROBOTRACER_MARCAS] = {ADC_CHANNEL0, ADC_CHANNEL1, ADC_CHANNEL2, ADC_CHANNEL3, ADC_CHANNEL4, ADC_CHANNEL5, ADC_CHANNEL6, ADC_CHANNEL7, ADC_CHANNEL14, ADC_CHANNEL15, ADC_CHANNEL8, ADC_CHANNEL9, ADC_CHANNEL10, ADC_CHANNEL11, ADC_CHANNEL12, ADC_CHANNEL13};

static volatile uint16_t sensores_raw[NUM_SENSORES_MAX];

static uint16_t sensores_max_linefollower[NUM_SENSORES_LINEFOLLOWER];
static uint16_t sensores_min_linefollower[NUM_SENSORES_LINEFOLLOWER];
static uint16_t sensores_umb_linefollower[NUM_SENSORES_LINEFOLLOWER];

static uint16_t sensores_max_robotracer[NUM_SENSORES_ROBOTRACER + NUM_SENSORES_ROBOTRACER_MARCAS];
static uint16_t sensores_min_robotracer[NUM_SENSORES_ROBOTRACER + NUM_SENSORES_ROBOTRACER_MARCAS];
static uint16_t sensores_umb_robotracer[NUM_SENSORES_ROBOTRACER + NUM_SENSORES_ROBOTRACER_MARCAS];

static volatile int32_t line_position = 0;
static uint32_t ultimaLinea = 0;
static bool left_mark = false;
static bool right_mark = false;
static int32_t ticks_ultima_interseccion = 0;

static uint32_t millis_emergency_stop = 0;

/**
 * @brief Inicializa los sensores cuando se enciende el robot sin calibrarlos a partir de una calibración previa
 *
 */
static void assign_sensors_calibrations(void) {
  if (get_config_robot() == CONFIG_ROBOT_LINEFOLLOWER) {
    uint16_t *eeprom_data = eeprom_get_data();
    for (uint16_t i = DATA_INDEX_SENSORS_MAX; i < NUM_SENSORES_MAX; i++) {
      sensores_max_linefollower[i] = eeprom_data[i];
    }
    for (uint16_t i = DATA_INDEX_SENSORS_MIN; i < DATA_INDEX_SENSORS_MIN + NUM_SENSORES_MAX; i++) {
      sensores_min_linefollower[i - DATA_INDEX_SENSORS_MIN] = eeprom_data[i];
    }
    for (uint16_t i = DATA_INDEX_SENSORS_UMB; i < DATA_INDEX_SENSORS_UMB + NUM_SENSORES_MAX; i++) {
      sensores_umb_linefollower[i - DATA_INDEX_SENSORS_UMB] = eeprom_data[i];
    }
  }
}

void print_sensors_calibrations(void) {
  if (get_config_robot() == CONFIG_ROBOT_LINEFOLLOWER) {
    for (int sensor = 0; sensor < get_sensors_line_num(); sensor++) {
      printf("sensores_max_linefollower[%d] = %d;\n", sensor, sensores_max_linefollower[sensor]);
      printf("sensores_min_linefollower[%d] = %d;\n", sensor, sensores_min_linefollower[sensor]);
      printf("sensores_umb_linefollower[%d] = %d;\n", sensor, sensores_umb_linefollower[sensor]);
    }
  } else {
    for (int sensor = 0; sensor < get_sensors_num(); sensor++) {
      printf("sensores_max_robotracer[%d] = %d;\n", sensor, sensores_max_robotracer[sensor]);
      printf("sensores_min_robotracer[%d] = %d;\n", sensor, sensores_min_robotracer[sensor]);
      printf("sensores_umb_robotracer[%d] = %d;\n", sensor, sensores_umb_robotracer[sensor]);
    }
  }
}

uint8_t *get_sensors(void) {
  if (get_config_robot() == CONFIG_ROBOT_LINEFOLLOWER) {
    return sensores_linefollower;
  } else {
    return sensores_robotracer;
  }
}

uint8_t get_sensors_num(void) {
  if (get_config_robot() == CONFIG_ROBOT_LINEFOLLOWER) {
    return NUM_SENSORES_LINEFOLLOWER;
  } else {
    return NUM_SENSORES_ROBOTRACER + NUM_SENSORES_ROBOTRACER_MARCAS;
  }
}

uint8_t get_sensors_line_num(void) {
  if (get_config_robot() == CONFIG_ROBOT_LINEFOLLOWER) {
    return NUM_SENSORES_LINEFOLLOWER;
  } else {
    return NUM_SENSORES_ROBOTRACER;
  }
}

volatile uint16_t *get_sensors_raw(void) {
  return sensores_raw;
}

uint16_t get_sensor_raw(uint8_t pos) {
  if (pos < NUM_SENSORES_MAX) {
    return sensores_raw[pos];
  } else {
    return 0;
  }
}

uint16_t get_sensor_calibrated(uint8_t pos) {
  if (pos < NUM_SENSORES_MAX) {
    uint16_t sensor_calibrado = get_sensor_raw(pos);

    if (get_config_robot() == CONFIG_ROBOT_LINEFOLLOWER) {
      if (sensor_calibrado >= sensores_umb_linefollower[pos]) {
        sensor_calibrado = LECTURA_MAXIMO_SENSORES_LINEA;
      } else {
        sensor_calibrado = LECTURA_MINIMO_SENSORES_LINEA;
      }
    } else {
      if (sensor_calibrado >= sensores_umb_robotracer[pos]) {
        sensor_calibrado = LECTURA_MAXIMO_SENSORES_LINEA;
      } else {
        sensor_calibrado = LECTURA_MINIMO_SENSORES_LINEA;
      }
    }

    if (get_config_line() == CONFIG_LINE_BLACK) {
      return sensor_calibrado;
    } else {
      return LECTURA_MAXIMO_SENSORES_LINEA - sensor_calibrado;
    }
  } else {
    return 0;
  }
}

void calibrate_sensors(void) {
  assign_sensors_calibrations();
  bool use_eeprom_calibration = true;
  while (!get_start_btn()) {
    set_neon_heartbeat();
    if (get_menu_mode_btn()) {
      use_eeprom_calibration = !use_eeprom_calibration;
      set_status_led(use_eeprom_calibration);
      while (get_menu_mode_btn()) {
      }
    }
    set_status_led(use_eeprom_calibration);
  }
  while (get_start_btn()) {
    set_neon_heartbeat();
    warning_status_led(75);
  }

  if (use_eeprom_calibration) {
    bool sensorsMinChecked[get_sensors_num()];
    bool sensorsMaxChecked[get_sensors_num()];
    for (uint8_t sensor = 0; sensor < get_sensors_num(); sensor++) {
      sensorsMinChecked[sensor] = false;
      sensorsMaxChecked[sensor] = false;
    }

    uint8_t countSensorsChecked = 0;
    uint32_t millisSensorsChecked = 0;
    while (!get_start_btn() && (countSensorsChecked < get_sensors_num() || get_clock_ticks() - millisSensorsChecked < 500)) {
      for (uint8_t sensor = 0; sensor < get_sensors_num(); sensor++) {
        if (get_config_robot() == CONFIG_ROBOT_LINEFOLLOWER) {
          if (abs(get_sensor_raw(sensor) - sensores_min_linefollower[sensor]) < 200) {
            sensorsMinChecked[sensor] = true;
          }
          if (abs(get_sensor_raw(sensor) - sensores_max_linefollower[sensor]) < 200 && sensorsMinChecked[sensor]) {
            sensorsMaxChecked[sensor] = true;
          }
        }
      }

      countSensorsChecked = 0;
      for (uint8_t sensor = 0; sensor < get_sensors_num(); sensor++) {
        if (sensorsMinChecked[sensor] && sensorsMaxChecked[sensor]) {
          countSensorsChecked++;
        }
      }

      if (countSensorsChecked == 0) {
        set_RGB_color(125, 0, 0);
      } else if (countSensorsChecked < get_sensors_num()) {
        set_RGB_color(125, 125, 0);
      } else {
        set_RGB_color(0, 125, 0);
        if (millisSensorsChecked == 0) {
          millisSensorsChecked = get_clock_ticks();
        }
      }
    }
  } else {
    delay(1000);

    // Resetear los valores máximos, mínimos y umbrales
    for (uint8_t sensor = 0; sensor < NUM_SENSORES_MAX; sensor++) {
      sensores_max_linefollower[sensor] = LECTURA_MINIMO_SENSORES_LINEA;
      sensores_min_linefollower[sensor] = LECTURA_MAXIMO_SENSORES_LINEA;
      sensores_umb_linefollower[sensor] = LECTURA_MINIMO_SENSORES_LINEA;

      sensores_max_robotracer[sensor] = LECTURA_MINIMO_SENSORES_LINEA;
      sensores_min_robotracer[sensor] = LECTURA_MAXIMO_SENSORES_LINEA;
      sensores_umb_robotracer[sensor] = LECTURA_MINIMO_SENSORES_LINEA;
    }

    uint32_t ms_inicio = get_clock_ticks();
    while (ms_inicio + MS_CALIBRACION_LINEA >= get_clock_ticks()) {
      for (int sensor = 0; sensor < get_sensors_num(); sensor++) {
        if (get_config_robot() == CONFIG_ROBOT_LINEFOLLOWER) {
          if (sensores_raw[sensor] < sensores_min_linefollower[sensor]) {
            sensores_min_linefollower[sensor] = sensores_raw[sensor];
          }
          if (sensores_raw[sensor] > sensores_max_linefollower[sensor]) {
            sensores_max_linefollower[sensor] = sensores_raw[sensor];
          }
        } else {
          if (sensores_raw[sensor] < sensores_min_robotracer[sensor]) {
            sensores_min_robotracer[sensor] = sensores_raw[sensor];
          }
          if (sensores_raw[sensor] > sensores_max_robotracer[sensor]) {
            sensores_max_robotracer[sensor] = sensores_raw[sensor];
          }
        }
      }

      set_RGB_rainbow();
    }

    bool calibrationOK = true;
    bool marksOK = true;
    if (get_config_robot() == CONFIG_ROBOT_LINEFOLLOWER) {
      for (int sensor = 0; sensor < get_sensors_num(); sensor++) {
        if (abs(sensores_max_linefollower[sensor] - sensores_min_linefollower[sensor]) < 1000) {
          calibrationOK = false;
        }
        sensores_umb_linefollower[sensor] = sensores_min_linefollower[sensor] + ((sensores_max_linefollower[sensor] - sensores_min_linefollower[sensor]) * 2 / 3);
      }
    } else {
      for (int sensor = 0; sensor < get_sensors_num(); sensor++) {
        if (abs(sensores_max_robotracer[sensor] - sensores_min_robotracer[sensor]) < 1000) {
          if (sensor < get_sensors_line_num()) {
            calibrationOK = false;
          } else {
            marksOK = false;
          }
        }
        sensores_umb_robotracer[sensor] = (sensores_max_robotracer[sensor] + sensores_min_robotracer[sensor]) * 2 / 3;
      }
    }

    eeprom_set_data(DATA_INDEX_SENSORS_MAX, sensores_max_linefollower, NUM_SENSORES_MAX);
    eeprom_set_data(DATA_INDEX_SENSORS_MIN, sensores_min_linefollower, NUM_SENSORES_MAX);
    eeprom_set_data(DATA_INDEX_SENSORS_UMB, sensores_umb_linefollower, NUM_SENSORES_MAX);

    if (calibrationOK && marksOK) {
      set_RGB_color(0, 100, 0);
      delay(500);
    } else {
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
    }
  }
  set_RGB_color(0, 0, 0);
  delay(250);
}

int32_t get_sensor_line_position(void) {
  return line_position;
}

void calc_sensor_line_position(void) {
  uint32_t suma_sensores_ponderados = 0;
  uint32_t suma_sensores = 0;
  uint8_t sensores_detectando = 0;
  uint8_t sensores_detectando_sin_filtro = 0;
  // Obtener los sensores importantes para el cálculo de posición
  int8_t sensor_ini_linea = -1;
  int8_t sensor_fin_linea = -1;
  int8_t sensor_inicial = round(map(line_position, -1000, 1000, -1, get_sensors_line_num()));
  if (sensor_inicial <= 0) {
    sensor_ini_linea = 0;
    sensor_fin_linea = 3;
  } else if (sensor_inicial >= get_sensors_line_num() - 1) {
    sensor_ini_linea = get_sensors_line_num() - 4;
    sensor_fin_linea = get_sensors_line_num() - 1;
  } else {
    sensor_ini_linea = sensor_inicial - 2;
    sensor_fin_linea = sensor_inicial + 1;
    if (sensor_ini_linea < 0) {
      sensor_fin_linea += abs(sensor_ini_linea);
      sensor_ini_linea = 0;
    }
    if (sensor_fin_linea >= get_sensors_line_num()) {
      sensor_ini_linea -= abs(sensor_fin_linea - get_sensors_line_num());
      sensor_fin_linea = get_sensors_line_num() - 1;
    }
  }

  // printf("%d - %d\t|\t", sensor_ini_linea, sensor_fin_linea);

  for (uint8_t sensor = 0; sensor < get_sensors_line_num(); sensor++) {
    uint16_t sensor_value = get_sensor_calibrated(sensor);

    if (get_config_robot() == CONFIG_ROBOT_LINEFOLLOWER) {
      if (sensor_value >= sensores_umb_linefollower[sensor]) {
        sensores_detectando_sin_filtro++;
      }
    } else {
      if (sensor_value >= sensores_umb_robotracer[sensor]) {
        sensores_detectando_sin_filtro++;
      }
    }
    if (sensor < sensor_ini_linea || sensor > sensor_fin_linea) {
      sensor_value = LECTURA_MINIMO_SENSORES_LINEA;
    }

    if (get_config_robot() == CONFIG_ROBOT_LINEFOLLOWER) {

      if (sensor_value >= sensores_umb_linefollower[sensor]) {
        sensores_detectando++;
      }
    } else {
      if (sensor_value >= sensores_umb_robotracer[sensor]) {
        sensores_detectando++;
      }
    }
    suma_sensores_ponderados += (sensor + 1) * sensor_value * 1000;
    suma_sensores += sensor_value;
  }

  if (sensores_detectando > 3) {
    ticks_ultima_interseccion = (get_encoder_left_total_ticks() + get_encoder_right_total_ticks()) / 2;
  }

  if (sensores_detectando > 0 && sensores_detectando_sin_filtro < get_sensors_line_num() / 2) {
    ultimaLinea = get_clock_ticks();
  } else if (is_competicion_iniciada()) {
    if (get_clock_ticks() > (ultimaLinea + get_offtrack_time())) {
      emergency_stop();
    }
    if (abs(line_position) < 800) {
      return;
    }
  }

  int32_t posicion_max = ((1000 * (get_sensors_line_num() + 1)) / 2);

  int32_t posicion;
  if (sensores_detectando > 0) {
    posicion = (suma_sensores_ponderados / suma_sensores) - posicion_max;
  } else {
    posicion = (line_position >= 0) ? posicion_max : -posicion_max;
  }
  posicion = map(posicion, -posicion_max, posicion_max, -1000, 1000);

  line_position = posicion;
}

void check_side_marks(void) {
  bool side_marks[4] = {false, false, false, false};
  for (uint8_t sensor = get_sensors_line_num(); sensor < get_sensors_num(); sensor++) {
    side_marks[sensor] = get_sensor_calibrated(sensor) >= sensores_umb_robotracer[sensor];
  }
  bool left = side_marks[2] || side_marks[3];
  bool right = side_marks[0] || side_marks[1];

  bool linea_simple = (abs(max_likelihood_counter_diff((get_encoder_left_total_ticks() + get_encoder_right_total_ticks()) / 2, ticks_ultima_interseccion)) > MICROMETERS_PER_METER * 0.10 / get_micrometers_per_tick() || ticks_ultima_interseccion == 0);

  left_mark = left && !(left && right) /* && linea_simple */;
  right_mark = right && !(left && right) && linea_simple;
}

bool is_left_mark(void) {
  return left_mark;
}

bool is_right_mark(void) {
  return right_mark;
}

uint32_t get_millis_emergency_stop(void) {
  return millis_emergency_stop;
}

void reset_millis_emergency_stop(void) {
  millis_emergency_stop = 0;
}

void emergency_stop(void) {
  set_competicion_iniciada(false);
  pause_pid_speed_timer();
  line_position = 0;
  millis_emergency_stop = get_clock_ticks();
}
#include <sensors.h>

#define NUM_SENSORES_LINEFOLLOWER 12
#define NUM_SENSORES_ROBOTRACER_LARGO 6
#define NUM_SENSORES_ROBOTRACER_CORTO 10
#define NUM_SENSORES_ROBOTRACER_MARCAS 4
#define NUM_SENSORES_MAX NUM_SENSORES_LINEFOLLOWER + NUM_SENSORES_ROBOTRACER_MARCAS

static uint8_t sensores_linefollower[NUM_SENSORES_LINEFOLLOWER] = {ADC_CHANNEL0, ADC_CHANNEL1, ADC_CHANNEL2, ADC_CHANNEL3, ADC_CHANNEL4, ADC_CHANNEL5, ADC_CHANNEL6, ADC_CHANNEL7, ADC_CHANNEL14, ADC_CHANNEL15, ADC_CHANNEL8, ADC_CHANNEL9};

static uint8_t sensores_robotracer[NUM_SENSORES_ROBOTRACER_CORTO + NUM_SENSORES_ROBOTRACER_MARCAS] = {ADC_CHANNEL2, ADC_CHANNEL3, ADC_CHANNEL4, ADC_CHANNEL5, ADC_CHANNEL6, ADC_CHANNEL7, ADC_CHANNEL14, ADC_CHANNEL15, ADC_CHANNEL8, ADC_CHANNEL9, ADC_CHANNEL10, ADC_CHANNEL11, ADC_CHANNEL12, ADC_CHANNEL13};

static volatile uint16_t sensores_raw[NUM_SENSORES_MAX];

static uint16_t sensores_max_linefollower[NUM_SENSORES_MAX];
static uint16_t sensores_min_linefollower[NUM_SENSORES_MAX];
static uint16_t sensores_umb_linefollower[NUM_SENSORES_MAX];

static uint16_t sensores_max_robotracer_corto[NUM_SENSORES_MAX];
static uint16_t sensores_min_robotracer_corto[NUM_SENSORES_MAX];
static uint16_t sensores_umb_robotracer_corto[NUM_SENSORES_MAX];

static uint16_t sensores_max_robotracer_largo[NUM_SENSORES_MAX];
static uint16_t sensores_min_robotracer_largo[NUM_SENSORES_MAX];
static uint16_t sensores_umb_robotracer_largo[NUM_SENSORES_MAX];

static int32_t line_position = 0;
static uint32_t ultimaLinea = 0;
static bool left_mark = false;
static bool right_mark = false;
static int32_t ticks_ultima_interseccion = 0;

static uint8_t tipo_morro;
static bool morro_auto = false;

/**
 * @brief Inicializa los sensores cuando se enciende el robot sin calibrarlos a partir de una calibración previa
 * 
 */
static void assign_sensors_calibrations() {
  sensores_max_linefollower[0] = 3735;
  sensores_min_linefollower[0] = 289;
  sensores_umb_linefollower[0] = 2586;
  sensores_max_linefollower[1] = 3918;
  sensores_min_linefollower[1] = 421;
  sensores_umb_linefollower[1] = 2752;
  sensores_max_linefollower[2] = 3911;
  sensores_min_linefollower[2] = 606;
  sensores_umb_linefollower[2] = 2809;
  sensores_max_linefollower[3] = 3865;
  sensores_min_linefollower[3] = 243;
  sensores_umb_linefollower[3] = 2657;
  sensores_max_linefollower[4] = 3867;
  sensores_min_linefollower[4] = 260;
  sensores_umb_linefollower[4] = 2664;
  sensores_max_linefollower[5] = 2883;
  sensores_min_linefollower[5] = 624;
  sensores_umb_linefollower[5] = 2130;
  sensores_max_linefollower[6] = 3743;
  sensores_min_linefollower[6] = 221;
  sensores_umb_linefollower[6] = 2569;
  sensores_max_linefollower[7] = 3881;
  sensores_min_linefollower[7] = 249;
  sensores_umb_linefollower[7] = 2670;
  sensores_max_linefollower[8] = 3841;
  sensores_min_linefollower[8] = 222;
  sensores_umb_linefollower[8] = 2634;
  sensores_max_linefollower[9] = 2905;
  sensores_min_linefollower[9] = 562;
  sensores_umb_linefollower[9] = 2124;
  sensores_max_linefollower[10] = 3762;
  sensores_min_linefollower[10] = 242;
  sensores_umb_linefollower[10] = 2588;
  sensores_max_linefollower[11] = 3887;
  sensores_min_linefollower[11] = 326;
  sensores_umb_linefollower[11] = 2700;
}

void print_sensors_calibrations() {
  if (get_config_robot() == CONFIG_ROBOT_LINEFOLLOWER) {
    for (int sensor = 0; sensor < get_sensors_line_num(); sensor++) {
      printf("sensores_max_linefollower[%d] = %d;\n", sensor, sensores_max_linefollower[sensor]);
      printf("sensores_min_linefollower[%d] = %d;\n", sensor, sensores_min_linefollower[sensor]);
      printf("sensores_umb_linefollower[%d] = %d;\n", sensor, sensores_umb_linefollower[sensor]);
    }
  } else {
    activar_morro_corto();
    for (int sensor = 0; sensor < get_sensors_num(); sensor++) {
      printf("sensores_max_robotracer_corto[%d] = %d;\n", sensor, sensores_max_robotracer_corto[sensor]);
      printf("sensores_min_robotracer_corto[%d] = %d;\n", sensor, sensores_min_robotracer_corto[sensor]);
      printf("sensores_umb_robotracer_corto[%d] = %d;\n", sensor, sensores_umb_robotracer_corto[sensor]);
    }
    /* activar_morro_largo();
    for (int sensor = 2; sensor < get_sensors_line_num() + 2; sensor++) {
      printf("sensores_max_robotracer_largo[%d] = %d\n", sensor - 2, sensores_max_robotracer_largo[sensor]);
      printf("sensores_min_robotracer_largo[%d] = %d\n", sensor - 2, sensores_min_robotracer_largo[sensor]);
      printf("sensores_umb_robotracer_largo[%d] = %d\n", sensor - 2, sensores_umb_robotracer_largo[sensor]);
    }
    for (int sensor = get_sensors_line_num() + 4; sensor < get_sensors_num(); sensor++) {
      printf("sensores_max_robotracer_largo[%d] = %d\n", sensor - 4, sensores_max_robotracer_largo[sensor]);
      printf("sensores_min_robotracer_largo[%d] = %d\n", sensor - 4, sensores_min_robotracer_largo[sensor]);
      printf("sensores_umb_robotracer_largo[%d] = %d\n", sensor - 4, sensores_umb_robotracer_largo[sensor]);
    } 
    activar_morro_corto();*/
  }
}

uint8_t *get_sensors() {
  if (get_config_robot() == CONFIG_ROBOT_LINEFOLLOWER) {
    return sensores_linefollower;
  } else {
    return sensores_robotracer;
  }
}

uint8_t get_sensors_num() {
  if (get_config_robot() == CONFIG_ROBOT_LINEFOLLOWER) {
    return NUM_SENSORES_LINEFOLLOWER;
  } else {
    return NUM_SENSORES_ROBOTRACER_CORTO + NUM_SENSORES_ROBOTRACER_MARCAS;
  }
}

uint8_t get_sensors_line_num() {
  if (get_config_robot() == CONFIG_ROBOT_LINEFOLLOWER) {
    return NUM_SENSORES_LINEFOLLOWER;
  } else {
    if (tipo_morro == TIPO_MORRO_CORTO) {
      return NUM_SENSORES_ROBOTRACER_CORTO;
    } else {
      return NUM_SENSORES_ROBOTRACER_LARGO;
    }
  }
}

volatile uint16_t *get_sensors_raw() {
  return sensores_raw;
}

uint16_t get_sensor_raw(uint8_t pos) {
  if (pos < NUM_SENSORES_MAX) {
    if (get_config_robot() == CONFIG_ROBOT_LINEFOLLOWER) {
      return sensores_raw[pos];
    } else {
      if (tipo_morro == TIPO_MORRO_CORTO) {
        return sensores_raw[pos];
      } else {
        if (pos < get_sensors_line_num()) {
          // Se están obteniendo sensores de línea
          return sensores_raw[pos + 2];
        } else {
          // Se están obteniendo marcas y el morro corto deja "libres" dos sensores de cada lado
          return sensores_raw[pos + 2 + 2];
        }
      }
    }
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
      if (tipo_morro == TIPO_MORRO_CORTO) {
        if (sensor_calibrado >= sensores_umb_robotracer_corto[pos]) {
          sensor_calibrado = LECTURA_MAXIMO_SENSORES_LINEA;
        } else {
          sensor_calibrado = LECTURA_MINIMO_SENSORES_LINEA;
        }
      } else {
        if (pos < get_sensors_line_num()) {
          pos += 2;
        } else {
          pos += 4;
        }
        if (sensor_calibrado >= sensores_umb_robotracer_largo[pos]) {
          sensor_calibrado = LECTURA_MAXIMO_SENSORES_LINEA;
        } else {
          sensor_calibrado = LECTURA_MINIMO_SENSORES_LINEA;
        }
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

void calibrate_sensors() {
  bool skip_calibration = get_config_run() == CONFIG_RUN_RACE; // En modo carrera, se calibra automáticamente por defecto.
  bool pushFlag = false;
  while (!get_start_btn()) {
    set_neon_heartbeat();
    if (get_menu_mode_btn()) {
      if (!pushFlag) {
        skip_calibration = !skip_calibration;
        pushFlag = true;
      }
    } else {
      pushFlag = false;
    }
    set_status_led(skip_calibration);
  }
  while (get_start_btn()) {
    set_neon_heartbeat();
  }
  while (skip_calibration && !is_esc_inited()) {
  }
  set_neon_fade(0);
  set_status_led(false);
  if (skip_calibration) {
    assign_sensors_calibrations();

    bool sensorsChecked[NUM_SENSORES_MAX];
    for (uint8_t sensor = 0; sensor < get_sensors_num(); sensor++) {
      sensorsChecked[sensor] = false;
    }

    while (!get_start_btn()) {

      for (uint8_t sensor = 0; sensor < get_sensors_num(); sensor++) {
        if (get_config_robot() == CONFIG_ROBOT_LINEFOLLOWER) {
          if (get_sensor_calibrated(sensor) >= sensores_umb_linefollower[sensor]) {
            sensorsChecked[sensor] = true;
          }
        }
      }

      uint8_t countSensorsChecked = 0;
      for (uint8_t sensor = 0; sensor < get_sensors_num(); sensor++) {
        if (sensorsChecked[sensor]) {
          countSensorsChecked++;
        }
      }

      if (countSensorsChecked == 0) {
        set_RGB_color(125, 0, 0);
      } else if (countSensorsChecked < get_sensors_num()) {
        set_RGB_color(125, 125, 0);
      } else {
        set_RGB_color(0, 125, 0);
      }
    }

    return;
  }
  delay(1000);

  // Resetear los valores máximos, mínimos y umbrales
  for (uint8_t sensor = 0; sensor < NUM_SENSORES_MAX; sensor++) {
    sensores_max_linefollower[sensor] = LECTURA_MINIMO_SENSORES_LINEA;
    sensores_min_linefollower[sensor] = LECTURA_MAXIMO_SENSORES_LINEA;
    sensores_umb_linefollower[sensor] = LECTURA_MINIMO_SENSORES_LINEA;

    sensores_max_robotracer_largo[sensor] = LECTURA_MINIMO_SENSORES_LINEA;
    sensores_min_robotracer_largo[sensor] = LECTURA_MAXIMO_SENSORES_LINEA;
    sensores_umb_robotracer_largo[sensor] = LECTURA_MINIMO_SENSORES_LINEA;

    sensores_max_robotracer_corto[sensor] = LECTURA_MINIMO_SENSORES_LINEA;
    sensores_min_robotracer_corto[sensor] = LECTURA_MAXIMO_SENSORES_LINEA;
    sensores_umb_robotracer_corto[sensor] = LECTURA_MINIMO_SENSORES_LINEA;
  }

  bool morro_cambiado = false;
  uint32_t ms_inicio = get_clock_ticks();
  while (ms_inicio + MS_CALIBRACION_LINEA >= get_clock_ticks()) {
    for (int sensor = 0; sensor < NUM_SENSORES_MAX; sensor++) {
      // printf("%d\t", sensores_raw[sensor]);
      if (get_config_robot() == CONFIG_ROBOT_LINEFOLLOWER) {
        if (sensores_raw[sensor] < sensores_min_linefollower[sensor]) {
          sensores_min_linefollower[sensor] = sensores_raw[sensor];
        }
        if (sensores_raw[sensor] > sensores_max_linefollower[sensor]) {
          sensores_max_linefollower[sensor] = sensores_raw[sensor];
        }
      } else {
        if (tipo_morro == TIPO_MORRO_CORTO) {
          if (sensores_raw[sensor] < sensores_min_robotracer_corto[sensor]) {
            sensores_min_robotracer_corto[sensor] = sensores_raw[sensor];
          }
          if (sensores_raw[sensor] > sensores_max_robotracer_corto[sensor]) {
            sensores_max_robotracer_corto[sensor] = sensores_raw[sensor];
          }
        } else {
          if (sensores_raw[sensor] < sensores_min_robotracer_largo[sensor]) {
            sensores_min_robotracer_largo[sensor] = sensores_raw[sensor];
          }
          if (sensores_raw[sensor] > sensores_max_robotracer_largo[sensor]) {
            sensores_max_robotracer_largo[sensor] = sensores_raw[sensor];
          }
        }

        // Cambiar de tipo de morro para calibración
        if (ms_inicio + MS_CALIBRACION_LINEA / 2 >= get_clock_ticks() && !morro_cambiado) {
          toggle_tipo_morro();
          morro_cambiado = true;
        }
      }

      set_RGB_rainbow();
      if (skip_calibration) {
        set_motors_speed(20, -20);
      }
    }

    if (get_config_robot() == CONFIG_ROBOT_ROBOTRACER) {
      toggle_tipo_morro();
    }
  }
  if (skip_calibration) {
    set_motors_speed(0, 0);
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

    activar_morro_corto();
    for (int sensor = 0; sensor < get_sensors_num(); sensor++) {
      if (abs(sensores_max_robotracer_corto[sensor] - sensores_min_robotracer_corto[sensor]) < 1000) {
        if (sensor < get_sensors_line_num()) {
          calibrationOK = false;
        } else if (get_config_track() == CONFIG_TRACK_ROBOTRACER) {
          marksOK = false;
        }
      }
      sensores_umb_robotracer_corto[sensor] = (sensores_max_robotracer_corto[sensor] + sensores_min_robotracer_corto[sensor]) / 2.;
    }
    activar_morro_largo();
    for (int sensor = 2; sensor < get_sensors_line_num() + 2; sensor++) {
      if (abs(sensores_max_robotracer_largo[sensor] - sensores_min_robotracer_largo[sensor]) < 1000) {
        calibrationOK = false;
      }
      sensores_umb_robotracer_largo[sensor] = (sensores_max_robotracer_largo[sensor] + sensores_min_robotracer_largo[sensor]) / 2.;
    }
    for (int sensor = get_sensors_line_num() + 4; sensor < get_sensors_num(); sensor++) {
      if (get_config_track() == CONFIG_TRACK_ROBOTRACER && abs(sensores_max_robotracer_largo[sensor] - sensores_min_robotracer_largo[sensor]) < 1000) {
        marksOK = false;
      }
      sensores_umb_robotracer_largo[sensor] = (sensores_max_robotracer_largo[sensor] + sensores_min_robotracer_largo[sensor]) / 2.;
    }
  }

  print_sensors_calibrations();

  // Establece el tipo de morro por defecto
  if (get_config_robot() == CONFIG_ROBOT_ROBOTRACER) {
    activar_morro_corto();
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
      if (tipo_morro == TIPO_MORRO_CORTO) {
        if (sensor_value >= sensores_umb_robotracer_corto[sensor]) {
          sensores_detectando_sin_filtro++;
        }
      } else {
        if (sensor_value >= sensores_umb_robotracer_largo[sensor + 2]) {
          sensores_detectando_sin_filtro++;
        }
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

      if (tipo_morro == TIPO_MORRO_CORTO) {
        if (sensor_value >= sensores_umb_robotracer_corto[sensor]) {
          sensores_detectando++;
        }
      } else {
        if (sensor_value >= sensores_umb_robotracer_largo[sensor + 2]) {
          sensores_detectando++;
        }
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

void check_side_marks() {
  bool side_marks[4] = {false, false, false, false};
  for (uint8_t sensor = get_sensors_line_num(); sensor < get_sensors_num(); sensor++) {
    if (tipo_morro == TIPO_MORRO_CORTO) {
      side_marks[sensor - get_sensors_line_num()] = get_sensor_calibrated(sensor) >= sensores_umb_robotracer_corto[sensor];
    } else {
      side_marks[sensor - get_sensors_line_num()] = get_sensor_calibrated(sensor) >= sensores_umb_robotracer_largo[sensor];
    }
  }
  bool left = side_marks[2] || side_marks[3];
  bool right = side_marks[0] || side_marks[1];

  bool linea_simple = (abs(max_likelihood_counter_diff((get_encoder_left_total_ticks() + get_encoder_right_total_ticks()) / 2, ticks_ultima_interseccion)) > MICROMETERS_PER_METER * 0.10 / get_micrometers_per_tick() || ticks_ultima_interseccion == 0);

  left_mark = left && !(left && right) /* && linea_simple */;
  right_mark = right && !(left && right) && linea_simple;
}

bool is_left_mark() {
  return left_mark;
}

bool is_right_mark() {
  return right_mark;
}

uint8_t get_tipo_morro() {
  return tipo_morro;
}

void set_morro_auto(bool active) {
  morro_auto = active;
}

bool is_morro_auto() {
  return morro_auto;
}

void toggle_tipo_morro() {
  if (tipo_morro == TIPO_MORRO_LARGO) {
    activar_morro_corto();
    set_status_led(true);
  } else {
    activar_morro_largo();
    set_status_led(false);
  }
}

void activar_morro_largo() {
  gpio_clear(GPIOA, GPIO0);
  gpio_set(GPIOA, GPIO1);
  tipo_morro = TIPO_MORRO_LARGO;
}

void activar_morro_corto() {
  gpio_clear(GPIOA, GPIO1);
  gpio_set(GPIOA, GPIO0);
  tipo_morro = TIPO_MORRO_CORTO;
}

void emergency_stop() {
  set_competicion_iniciada(false);
  pause_pid_speed_timer();
  line_position = 0;
}
#include <sensors.h>

#define NUM_SENSORS 16
#define NUM_SENSORS_LINE 12

static uint8_t sensores[NUM_SENSORS] = {ADC_CHANNEL0, ADC_CHANNEL1, ADC_CHANNEL2, ADC_CHANNEL3, ADC_CHANNEL4, ADC_CHANNEL5, ADC_CHANNEL6, ADC_CHANNEL7, ADC_CHANNEL14, ADC_CHANNEL15, ADC_CHANNEL8, ADC_CHANNEL9, ADC_CHANNEL10, ADC_CHANNEL11, ADC_CHANNEL12, ADC_CHANNEL13};
static volatile uint16_t sensores_raw[NUM_SENSORS];
static uint16_t sensores_max[NUM_SENSORS];
static uint16_t sensores_min[NUM_SENSORS];
static uint16_t sensores_umb[NUM_SENSORS];
static int32_t line_position;

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
  if (pos + 1 > NUM_SENSORS) {
    return 0;
  } else {
    return sensores_raw[pos];
  }
}

uint16_t get_sensor_calibrated(uint8_t pos) {
  if (pos + 1 > NUM_SENSORS) {
    return 0;
  } else {
    uint16_t sensor_calibrado = sensores_raw[pos];
    if (sensor_calibrado >= sensores_umb[pos]) {
      sensor_calibrado = LECTURA_MAXIMO_SENSORES_LINEA;
    } else {
      sensor_calibrado = LECTURA_MINIMO_SENSORES_LINEA;
    }
    if (LINEA == LINEA_BLANCA) {
      return LECTURA_MAXIMO_SENSORES_LINEA - sensor_calibrado;
    } else {
      return sensor_calibrado;
    }
  }
}

void calibrate_sensors() {
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
    }
  }

  for (int sensor = 0; sensor < NUM_SENSORS; sensor++) {
    sensores_umb[sensor] = (sensores_max[sensor] + sensores_min[sensor]) / 2.;
  }
  set_RGB_color(0,0,0);
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

  //TODO: comprobar tiempo sin pista para detener automáticamente el robot.
  int32_t posicion;
  if (sensores_detectando > 0) {
    posicion = ((suma_sensores_ponderados / suma_sensores) - ((NUM_SENSORS_LINE + 1) * 1000) / 2);
  } else {
    posicion = (line_position >= 0) ? ((1000 * (NUM_SENSORS_LINE + 1)) / 2) : -((1000 * (NUM_SENSORS_LINE + 1)) / 2);
  }

  line_position = posicion;
}
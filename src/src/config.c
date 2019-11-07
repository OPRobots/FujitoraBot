#include <config.h>

uint16_t _run_mode;
uint16_t _speed_mode;
uint16_t _track_mode;

/**
 * @brief Establece la configuración de Carrera
 * 
 * @param run_mode CONFIG_RUN_RACE | CONFIG_RUN_DEBUG
 */
static void set_config_run(uint16_t run_mode) {
  _run_mode = run_mode;
}

/**
 * @brief Establece la configuración de Velocidad
 * 
 * @param speed_mode CONFIG_SPEED_MS | CONFIG_SPEED_PWM
 */
static void set_config_speed(uint16_t speed_mode) {
  _speed_mode = speed_mode;
}

/**
 * @brief Establece la configuración de Pista
 * 
 * @param track_mode CONFIG_TRACK_ROBOTRACER | CONFIG_TRACK_LINEFOLLOWER
 */
static void set_config_track(uint16_t track_mode) {
  _track_mode = track_mode;
}

/**
 * @brief Obtiene la configuración de Carrera
 * 
 * @return uint16_t CONFIG_RUN_RACE | CONFIG_RUN_DEBUG
 */
uint16_t get_config_run(void) {
  return _run_mode;
}

/**
 * @brief Obtiene la configuración de Velocidad
 * 
 * @return uint16_t CONFIG_SPEED_MS | CONFIG_SPEED_PWM
 */
uint16_t get_config_speed(void) {
  return _speed_mode;
}

/**
 * @brief Obtiene la configuración de Pista
 * 
 * @return uint16_t CONFIG_TRACK_ROBOTRACER | CONFIG_TRACK_LINEFOLLOWER
 */
uint16_t get_config_track(void) {
  return _track_mode;
}

/**
 * @brief Establece todas las configuraciones (CARRERA, VELOCIDAD, PISTA) en funcion de los Switches
 * 
 */
void set_all_configs(void) {
  if (get_swtich_1()) {
    set_config_run(CONFIG_RUN_RACE);
  } else {
    set_config_run(CONFIG_RUN_DEBUG);
  }
  if (get_swtich_2()) {
    set_config_speed(CONFIG_SPEED_MS);
  } else {
    set_config_speed(CONFIG_SPEED_PWM);
  }
  if (get_swtich_3()) {
    set_config_track(CONFIG_TRACK_ROBOTRACER);
  } else {
    set_config_track(CONFIG_TRACK_LINEFOLLOWER);
  }
}

/**
 * @brief Calcula el tiempo máximo fuera de pista en función de la configuración de carrera/debug
 * 
 * @return uint16_t Tiempo máximo fuera de pista
 */
uint16_t get_offtrack_time(void){
  if(get_config_run() == CONFIG_RUN_RACE){
    return (uint16_t)(TIEMPO_SIN_PISTA * 1.33);
  }else{
    return TIEMPO_SIN_PISTA;
  }
}

/**
 * @brief Calcula el tiempo máximo fuera de pista en función de la configuración de carrera/debug
 * 
 * @return uint16_t Tiempo máximo fuera de pista
 */
uint16_t get_start_millis(void){
  if(get_config_run() == CONFIG_RUN_RACE){
    return MILLIS_INICIO_RUN;
  }else{
    return MILLIS_INICIO_DEBUG;
  }
}
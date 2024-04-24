#include "buttons.h"

/**
 * @brief Obtiene el estado del botón de inicio
 * 
 * @return bool
 */
bool get_start_btn(void) {
  bool state1 = (bool)gpio_get(GPIOB, GPIO15);
  if(!state1) {
    return false;
  }
  delay(50);
  bool state2 = (bool)gpio_get(GPIOB, GPIO15);
  return state1 && state2;
}

/**
 * @brief Obtiene el estado del botón de Menú Arriba
 * 
 * @return bool
 */
bool get_menu_up_btn(void) {
  bool state1 = (bool)gpio_get(GPIOC, GPIO11);
  if(!state1) {
    return false;
  }
  delay(50);
  bool state2 = (bool)gpio_get(GPIOC, GPIO11);
  return state1 && state2;
}

/**
 * @brief Obtiene el estado del botón de Menú Abajo
 * 
 * @return bool
 */
bool get_menu_down_btn(void) {
  return false;
  // bool state1 = (bool)gpio_get(GPIOA, GPIO15);
  // if(!state1) {
  //   return false;
  // }
  // delay(50);
  // bool state2 = (bool)gpio_get(GPIOA, GPIO15);
  // return state1 && state2;
}

/**
 * @brief Obtiene el estado del botón de Menú Modo
 * 
 * @return bool
 */
bool get_menu_mode_btn(void) {
  bool state1 = (bool)gpio_get(GPIOC, GPIO10);
  if(!state1) {
    return false;
  }
  delay(50);
  bool state2 = (bool)gpio_get(GPIOC, GPIO10);
  return state1 && state2;
}

/**
 * @brief Obtiene el estado del switch 1
 * 
 * @return bool
 */
bool get_swtich_1(void) {
  return (bool)gpio_get(GPIOB, GPIO14);
}

/**
 * @brief Obtiene el estado del switch 2
 * 
 * @return bool
 */
bool get_swtich_2(void) {
  return (bool)gpio_get(GPIOB, GPIO13);
}

/**
 * @brief Obtiene el estado del switch 3
 * 
 * @return bool
 */
bool get_swtich_3(void) {
  bool state1 = (bool)gpio_get(GPIOB, GPIO12);
  delay(50);
  bool state2 = (bool)gpio_get(GPIOB, GPIO12);
  return state1 && state2;
}


/**
 * @brief Obtiene el número decimal a partir del binario de los Switches
 * 
 * @return uint8_t decimal
 */
uint8_t get_switch_decimal(void) {
  uint8_t binario = 0;
  uint8_t decimal = 0;

  if (get_swtich_1()) {
    binario += 100;
  }
  if (get_swtich_2()) {
    binario += 10;
  }
  if (get_swtich_3()) {
    binario += 1;
  }

  uint8_t i = 0;
  uint8_t resto;
  while (binario != 0) {
    resto = binario % 10;
    binario /= 10;
    decimal += resto * pow(2, i);
    ++i;
  }
  return decimal;
}
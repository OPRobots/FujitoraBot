#include <buttons.h>

/**
 * @brief Obtiene el estado del botón de inicio
 * 
 * @return bool
 */
bool get_start_btn() {
  return (bool)gpio_get(GPIOB, GPIO15);
}

/**
 * @brief Obtiene el estado del botón de Menú Arriba
 * 
 * @return bool
 */
bool get_menu_up_btn() {
  return (bool)gpio_get(GPIOC, GPIO11);
}

/**
 * @brief Obtiene el estado del botón de Menú Abajo
 * 
 * @return bool
 */
bool get_menu_down_btn() {
  return (bool)gpio_get(GPIOA, GPIO15);
}

/**
 * @brief Obtiene el estado del botón de Menú Modo
 * 
 * @return bool
 */
bool get_menu_mode_btn() {
  return (bool)gpio_get(GPIOC, GPIO10);
}

/**
 * @brief Obtiene el estado del switch 1
 * 
 * @return bool
 */
bool get_swtich_1() {
  return (bool)gpio_get(GPIOB, GPIO14);
}

/**
 * @brief Obtiene el estado del switch 2
 * 
 * @return bool
 */
bool get_swtich_2() {
  return (bool)gpio_get(GPIOB, GPIO13);
}

/**
 * @brief Obtiene el estado del switch 3
 * 
 * @return bool
 */
bool get_swtich_3() {
  return (bool)gpio_get(GPIOB, GPIO12);
}


/**
 * @brief Obtiene el número decimal a partir del binario de los Switches
 * 
 * @return uint8_t decimal
 */
uint8_t get_switch_decimal() {
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
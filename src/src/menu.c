#include <menu.h>

uint8_t modoConfig = 0;
#define NUM_MODOS 3
int8_t valorConfig = 0;
#define NUM_VALORES 9

uint8_t velocidadBase = 0;
uint8_t velocidadVentiladorBase = 0;

static void handle_menu_mode() {
  if (modoConfig == 0) {
    set_status_led(false);
  } else {
    warning_status_led(90 + modoConfig * 75);
  }
}

static void handle_menu_value() {
  switch (modoConfig) {
    case 0: // DEBUG
      debug_from_switch();
      break;
    case 1: // VELOCIDAD
      switch (valorConfig) {
        case 0:
          set_RGB_color(0, 10, 10);
          velocidadBase = 0;
          break;
        case 1:
          set_RGB_color(0, 10, 0);
          velocidadBase = 30;
          break;
        case 2:
          set_RGB_color(0, 255, 0);
          velocidadBase = 44;
          break;
        case 3:
          set_RGB_color(10, 10, 0);
          velocidadBase = 50;
          break;
        case 4:
          set_RGB_color(255, 225, 0);
          velocidadBase = 56;
          break;
        case 5:
          set_RGB_color(10, 0, 0);
          velocidadBase = 58;
          break;
        case 6:
          set_RGB_color(255, 0, 0);
          velocidadBase = 66;
          break;
        case 7:
          set_RGB_color(10, 0, 10);
          velocidadBase = 74;
          break;
        case 8:
          set_RGB_color(255, 0, 255);
          velocidadBase = 82;
          break;
        case 9:
          set_RGB_color(255, 255, 255);
          velocidadBase = 90;
          break;
      }
      break;
    case 2: // VENTILADORES
      switch (valorConfig) {
        case 0:
          set_RGB_color(0, 10, 10);
          velocidadVentiladorBase = 0;
          break;
        case 1:
          set_RGB_color(0, 10, 0);
          velocidadVentiladorBase = 20;
          break;
        case 2:
          set_RGB_color(0, 255, 0);
          velocidadVentiladorBase = 30;
          break;
        case 3:
          set_RGB_color(10, 10, 0);
          velocidadVentiladorBase = 40;
          break;
        case 4:
          set_RGB_color(255, 225, 0);
          velocidadVentiladorBase = 50;
          break;
        case 5:
          set_RGB_color(10, 0, 0);
          velocidadVentiladorBase = 60;
          break;
        case 6:
          set_RGB_color(255, 0, 0);
          velocidadVentiladorBase = 70;
          break;
        case 7:
          set_RGB_color(10, 0, 10);
          velocidadVentiladorBase = 80;
          break;
        case 8:
          set_RGB_color(255, 0, 255);
          velocidadVentiladorBase = 90;
          break;
        case 9:
          set_RGB_color(255, 255, 255);
          velocidadVentiladorBase = 100;
          break;
      }
      break;
    case 3: // EXTRA
      break;
  }
}

void check_menu_button() {
  // Comprueba cambios del modo de configuración
  if (get_menu_mode_btn()) {
    modoConfig++;
    if (modoConfig > NUM_MODOS) {
      modoConfig = 0;
    }
    while (get_menu_mode_btn()) {
      handle_menu_mode();
    };
    set_status_led(false);
    delay(50);
  }

  if (modoConfig == 0) {
    set_RGB_color(0, 0, 0);
    delay(50);
    handle_menu_value();
    return;
  }

  // Comprueba aumento de valor de configuración
  if (get_menu_up_btn()) {
    valorConfig++;
    if (valorConfig > NUM_VALORES) {
      valorConfig = NUM_VALORES;
    }
    while (get_menu_up_btn()) {
      handle_menu_value();
    };
    set_RGB_color(0, 0, 0);
    delay(50);
  }

  // Comprueba descenso de valor de configuración
  if (get_menu_down_btn()) {
    valorConfig--;
    if (valorConfig < 0) {
      valorConfig = 0;
    }
    while (get_menu_down_btn()) {
      handle_menu_value();
    };
    set_RGB_color(0, 0, 0);
    delay(50);
  }
}

uint8_t get_base_speed() {
  return velocidadBase;
}

uint8_t get_base_fan_speed() {
  return velocidadVentiladorBase;
}
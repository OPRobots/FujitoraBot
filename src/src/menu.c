#include <menu.h>

uint8_t modoConfig = 0;
#define NUM_MODOS 4
int8_t valorConfig[NUM_MODOS] = {0, 0, 0};
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
      switch (valorConfig[modoConfig]) {
        case 0:
          set_RGB_color(0, 10, 10);
          velocidadBase = 0;
          break;
        case 1:
          set_RGB_color(0, 10, 0);
          velocidadBase = 10;
          break;
        case 2:
          set_RGB_color(0, 255, 0);
          velocidadBase = 20;
          break;
        case 3:
          set_RGB_color(10, 10, 0);
          velocidadBase = 25;
          break;
        case 4:
          set_RGB_color(255, 225, 0);
          velocidadBase = 30;
          break;
        case 5:
          set_RGB_color(10, 0, 0);
          velocidadBase = 35;
          break;
        case 6:
          set_RGB_color(255, 0, 0);
          velocidadBase = 40;
          break;
        case 7:
          set_RGB_color(10, 0, 10);
          velocidadBase = 45;
          break;
        case 8:
          set_RGB_color(255, 0, 255);
          velocidadBase = 50;
          break;
        case 9:
          set_RGB_color(255, 255, 255);
          velocidadBase = 55;
          break;
      }
      break;
    case 2: // VENTILADORES
      switch (valorConfig[modoConfig]) {
        case 0:
          set_RGB_color(0, 10, 10);
          velocidadVentiladorBase = 10;
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
      switch (valorConfig[modoConfig]) {
        case 0:
          set_RGB_color(0, 10, 10);
          set_fans_speed(10, 10);
          break;
        case 1:
          set_RGB_color(0, 10, 0);
          set_fans_speed(20, 20);
          break;
        case 2:
          set_RGB_color(0, 255, 0);
          set_fans_speed(30, 30);
          break;
        case 3:
          set_RGB_color(10, 10, 0);
          set_fans_speed(40, 40);
          break;
        case 4:
          set_RGB_color(255, 225, 0);
          set_fans_speed(50, 50);
          break;
        case 5:
          set_RGB_color(10, 0, 0);
          set_fans_speed(60, 60);
          break;
        case 6:
          set_RGB_color(255, 0, 0);
          set_fans_speed(70, 70);
          break;
        case 7:
          set_RGB_color(10, 0, 10);
          set_fans_speed(80, 80);
          break;
        case 8:
          set_RGB_color(255, 0, 255);
          set_fans_speed(90, 90);
          break;
        case 9:
          set_RGB_color(255, 255, 255);
          set_fans_speed(100, 100);
          break;
      }
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
    valorConfig[modoConfig]++;
    if (valorConfig[modoConfig] > NUM_VALORES) {
      valorConfig[modoConfig] = NUM_VALORES;
    }
    while (get_menu_up_btn()) {
      handle_menu_value();
    };
    set_RGB_color(0, 0, 0);
    delay(50);
  }

  // Comprueba descenso de valor de configuración
  if (get_menu_down_btn()) {
    valorConfig[modoConfig]--;
    if (valorConfig[modoConfig] < 0) {
      valorConfig[modoConfig] = 0;
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
#include <menu.h>

uint8_t modoConfig = 0;
#define MODE_NOTHING 0
#define MODE_SPEED 1
#define MODE_FANS 2
#define MODE_TIPO_MORRO 3
#define MODE_DEBUG 4
#define NUM_MODOS_RACE 3
#define NUM_MODOS_DEBUG 5
int8_t valorConfig[NUM_MODOS_DEBUG] = {0, 0, 0, 0, 0};
#define NUM_VALORES 9
#define NUM_VALORES_TIPO_MORRO 2

uint8_t velocidadBase = 0;
float velocidadMsBase = 0;
uint8_t velocidadVentiladorBase = 0;

float velocidadRobotracerMsBase = 0;
float velocidadRobotracerMsStraight = 0;
float aceleracionCurvaRobotracerMss = 0.8f;
float aceleracionRobotracerMss = MAX_ACCEL_MS2;
float deceleracionRobotracerMss = MAX_BREAK_MS2;

static void handle_menu_mode() {
  switch (modoConfig) {
    case MODE_NOTHING:
      set_status_led(false);
      break;
    case MODE_SPEED:
      warning_status_led(50);
      break;
    case MODE_FANS:
      warning_status_led(200);
      break;
    case MODE_DEBUG:
      set_status_led(true);
      break;
  }
}

static void handle_menu_value() {
  switch (modoConfig) {
    case MODE_NOTHING:
      set_RGB_color(0, 0, 0);
      break;
    case MODE_SPEED:
      switch (valorConfig[modoConfig]) {
        case 0:
          set_RGB_color(0, 10, 10); // Cian
          velocidadBase = 0;
          velocidadMsBase = 0.0;
          break;
        case 1:
          set_RGB_color(0, 10, 0); // Verde ↓
          velocidadBase = 30;
          velocidadMsBase = 1.0;
          velocidadRobotracerMsBase = 1.25;
          aceleracionRobotracerMss = 10.0;
          deceleracionRobotracerMss = 10.0;
          velocidadRobotracerMsStraight = 2.5;
          aceleracionCurvaRobotracerMss = 0.8;
          break;
        case 2:
          set_RGB_color(0, 255, 0); // Verde ↑
          velocidadBase = 40;
          velocidadMsBase = 1.5;
          velocidadRobotracerMsBase = 1.25;
          aceleracionRobotracerMss = 10.0;
          deceleracionRobotracerMss = 10.0;
          velocidadRobotracerMsStraight = 4.0;
          aceleracionCurvaRobotracerMss = 0.8;
          break;
        case 3:
          set_RGB_color(10, 10, 0); // Amarillo ↓
          velocidadBase = 50;
          velocidadMsBase = 2.0;
          velocidadRobotracerMsBase = 1.5;
          aceleracionRobotracerMss = 10.0;
          deceleracionRobotracerMss = 10.0;
          velocidadRobotracerMsStraight = 3.5;
          aceleracionCurvaRobotracerMss = 0.85;
          break;
        case 4:
          set_RGB_color(255, 225, 0); // Amarillo ↑
          velocidadBase = 60;
          velocidadMsBase = 2.5;
          velocidadRobotracerMsBase = 1.5;
          aceleracionRobotracerMss = 10.0;
          deceleracionRobotracerMss = 10.0;
          velocidadRobotracerMsStraight = 4.5;
          aceleracionCurvaRobotracerMss = 0.85;
          break;
        case 5:
          set_RGB_color(10, 0, 0); // Rojo ↓
          velocidadBase = 70;
          velocidadMsBase = 3.0;
          velocidadRobotracerMsBase = 1.75;
          aceleracionRobotracerMss = 10.0;
          deceleracionRobotracerMss = 10.0;
          velocidadRobotracerMsStraight = 4.0;
          aceleracionCurvaRobotracerMss = 0.8;
          break;
        case 6:
          set_RGB_color(255, 0, 0); // Rojo ↑
          velocidadBase = 78;
          velocidadMsBase = 3.25;
          velocidadRobotracerMsBase = 1.75;
          aceleracionRobotracerMss = 10.0;
          deceleracionRobotracerMss = 10.0;
          velocidadRobotracerMsStraight = 5.0;
          aceleracionCurvaRobotracerMss = 0.8;
          break;
        case 7:
          set_RGB_color(10, 0, 10); // Haki ↓
          velocidadBase = 85;
          velocidadMsBase = 3.5;
          velocidadRobotracerMsBase = 2.0;
          aceleracionRobotracerMss = 10.0;
          deceleracionRobotracerMss = 10.0;
          velocidadRobotracerMsStraight = 4.5;
          aceleracionCurvaRobotracerMss = 0.8;
          break;
        case 8:
          set_RGB_color(255, 0, 255); // Haki ↑
          velocidadBase = 90;
          velocidadMsBase = 3.75;
          velocidadRobotracerMsBase = 2.0;
          aceleracionRobotracerMss = 10.0;
          deceleracionRobotracerMss = 10.0;
          velocidadRobotracerMsStraight = 5.0;
          aceleracionCurvaRobotracerMss = 0.8;
          break;
        case 9:
          set_RGB_color(255, 255, 255); // Ultra instinto
          velocidadBase = 95;
          velocidadMsBase = 4.0;
          break;
      }
      break;
    case MODE_FANS:
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
    case MODE_TIPO_MORRO:
      switch (valorConfig[modoConfig]) {
        case 0: // MORRO AUTOMATICO
          set_RGB_color(75, 0, 75);
          set_morro_auto(true);
          break;
        case 1: // MORRO CORTO
          set_RGB_color(0, 75, 0);
          activar_morro_corto();
          set_morro_auto(false);
          break;
        case 2: // MORRO LARGO
          set_RGB_color(75, 0, 0);
          activar_morro_largo();
          set_morro_auto(false);
          break;
      }
      break;
    case MODE_DEBUG:
      switch (valorConfig[modoConfig]) {
        case 0:
          set_RGB_color(0, 10, 10);
          break;
        case 1:
          set_RGB_color(0, 10, 0);
          break;
        case 2:
          set_RGB_color(0, 255, 0);
          break;
        case 3:
          set_RGB_color(10, 10, 0);
          break;
        case 4:
          set_RGB_color(255, 225, 0);
          break;
        case 5:
          set_RGB_color(10, 0, 0);
          break;
        case 6:
          set_RGB_color(255, 0, 0);
          break;
        case 7:
          set_RGB_color(10, 0, 10);
          break;
        case 8:
          set_RGB_color(255, 0, 255);
          break;
        case 9:
          set_RGB_color(255, 255, 255);
          break;
      }
      debug_from_config(valorConfig[modoConfig]);
      break;
  }
}

static uint8_t get_num_modos() {
  if (get_config_run() == CONFIG_RUN_RACE) {
    return NUM_MODOS_RACE; // NOTHING - VELOCIDAD - VENTILADORES
  } else {
    return NUM_MODOS_DEBUG; // NOTHING - VELOCIDAD - VENTILADORES - TIPO MORRO - DEBUG
  }
}

void check_menu_button() {
  handle_menu_mode();
  if (in_debug_mode()) {
    handle_menu_value();
  }

  // Comprueba cambios del modo de configuración
  if (get_menu_mode_btn()) {
    modoConfig++;
    if (modoConfig >= get_num_modos()) {
      modoConfig = 0;
    }
    while (get_menu_mode_btn()) {
      handle_menu_mode();
      handle_menu_value();
    };
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

    switch (modoConfig) {
      case MODE_TIPO_MORRO:
        if (valorConfig[modoConfig] > NUM_VALORES_TIPO_MORRO) {
          valorConfig[modoConfig] = NUM_VALORES_TIPO_MORRO;
        }
        break;
      default:
        if (valorConfig[modoConfig] > NUM_VALORES) {
          valorConfig[modoConfig] = NUM_VALORES;
        }
        break;
    }
    while (get_menu_up_btn()) {
      handle_menu_value();
      handle_menu_mode();
    };
    if (!in_debug_mode()) {
      set_RGB_color(0, 0, 0);
    }
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
      handle_menu_mode();
    };
    if (!in_debug_mode()) {
      set_RGB_color(0, 0, 0);
    }
    delay(50);
  }
}

uint8_t get_base_speed() {
  return velocidadBase;
}

float get_base_ms_speed() {
  if (get_config_track() == CONFIG_TRACK_LINEFOLLOWER) {
    return velocidadMsBase;
  } else {
    return velocidadRobotracerMsBase;
  }
}

float get_robotracer_straight_ms_speed() {
  return velocidadRobotracerMsStraight;
}
float get_base_turn_acceleration_mss() {
  if (get_config_track() == CONFIG_TRACK_LINEFOLLOWER) {
    return 0;
  } else {
    return aceleracionCurvaRobotracerMss;
  }
}

float get_base_deceleration_mss() {
  if (get_config_track() == CONFIG_TRACK_LINEFOLLOWER) {
    return MAX_BREAK_MS2;
  } else {
    return deceleracionRobotracerMss;
  }
}

float get_base_acceleration_mss() {
  if (get_config_track() == CONFIG_TRACK_LINEFOLLOWER) {
    return MAX_ACCEL_MS2;
  } else {
    return aceleracionRobotracerMss;
  }
}

uint8_t get_base_fan_speed() {
  return velocidadVentiladorBase;
}

bool in_debug_mode() {
  return modoConfig == MODE_DEBUG;
}

void reset_menu_mode() {
  modoConfig = MODE_NOTHING;
}
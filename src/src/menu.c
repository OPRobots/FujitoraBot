#include "menu.h"

uint8_t modoConfig = 0;
uint16_t valorConfig[NUM_MODOS_DEBUG] = {0, 0, 0, 0};

uint8_t velocidadBase = 0;
float velocidadMsBase = 0;
uint8_t velocidadVentiladorBase = 0;

float velocidadRobotracerMsBase = 0;
float velocidadRobotracerMsStraight = 0;
float aceleracionCurvaRobotracerMss = 0.8f;
float aceleracionRobotracerMss = MAX_ACCEL_MS2;
float deceleracionRobotracerMss = MAX_BREAK_MS2;

static void handle_menu_mode(void) {
  switch (modoConfig) {
    case MODE_NOTHING:
      set_status_led(false);
      break;
    case MODE_SPEED:
      warning_status_led(75);
      break;
    case MODE_FANS:
      warning_status_led(200);
      break;
    case MODE_DEBUG:
      set_status_led(true);
      break;
  }
}

static void handle_menu_value(void) {
  switch (modoConfig) {
    case MODE_NOTHING:
      set_RGB_color(0, 0, 0);
      if (get_menu_up_btn()) {
        debug_sensors_calibration();
        set_status_led(true);
      } else if (get_menu_down_btn()) {
        debug_log();
        set_status_led(true);
      } else {
        set_status_led(false);
      }
      break;
    case MODE_SPEED:

      //? Robotracer Config Variables
      /*
        velocidadRobotracerMsBase = 1.25;
        aceleracionRobotracerMss = 10.0;
        deceleracionRobotracerMss = 10.0;
        velocidadRobotracerMsStraight = 4.0;
        aceleracionCurvaRobotracerMss = 0.8;
      */
      switch (valorConfig[modoConfig]) {
        case 0:
          set_RGB_color(0, 10, 10); // Cian
          velocidadBase = 0;
          velocidadMsBase = 0.0;
          break;
        case 1:
          warning_RGB_color(0, 10, 10, MENU_WARNING_50); // Cian
          velocidadBase = 15;
          velocidadMsBase = 0.50;
          break;
        case 2:
          set_RGB_color(0, 80, 0); // Verde
          velocidadBase = 20;
          velocidadMsBase = 1.0;
          break;
        case 3:
          warning_RGB_color(0, 80, 0, MENU_WARNING_25); // Verde
          velocidadBase = 25;
          velocidadMsBase = 1.25;
          break;
        case 4:
          warning_RGB_color(0, 80, 0, MENU_WARNING_50); // Verde
          velocidadBase = 30;
          velocidadMsBase = 1.50;
          break;
        case 5:
          warning_RGB_color(0, 80, 0, MENU_WARNING_75); // Verde
          velocidadBase = 35;
          velocidadMsBase = 1.75;
          break;
        case 6:
          set_RGB_color(80, 80, 0); // Amarillo
          velocidadBase = 40;
          velocidadMsBase = 2.0;
          break;
        case 7:
          warning_RGB_color(80, 80, 0, MENU_WARNING_25); // Amarillo
          velocidadBase = 45;
          velocidadMsBase = 2.25;
          break;
        case 8:
          warning_RGB_color(80, 80, 0, MENU_WARNING_50); // Amarillo
          velocidadBase = 50;
          velocidadMsBase = 2.50;
          break;
        case 9:
          warning_RGB_color(80, 80, 0, MENU_WARNING_75); // Amarillo
          velocidadBase = 55;
          velocidadMsBase = 2.75;
          break;
        case 10:
          set_RGB_color(80, 0, 0); // Rojo
          velocidadBase = 60;
          velocidadMsBase = 3.0;
          break;
        case 11:
          warning_RGB_color(80, 0, 0, MENU_WARNING_25); // Rojo
          velocidadBase = 65;
          velocidadMsBase = 3.25;
          break;
        case 12:
          warning_RGB_color(80, 0, 0, MENU_WARNING_50); // Rojo
          velocidadBase = 70;
          velocidadMsBase = 3.50;
          break;
        case 13:
          warning_RGB_color(80, 0, 0, MENU_WARNING_75); // Rojo
          velocidadBase = 75;
          velocidadMsBase = 3.75;
          break;
        case 14:
          set_RGB_color(80, 0, 80); // Haki
          velocidadBase = 80;
          velocidadMsBase = 4.0;
          break;
        case 15:
          warning_RGB_color(80, 0, 80, MENU_WARNING_25); // Haki
          velocidadBase = 85;
          velocidadMsBase = 4.25;
          break;
        case 16:
          warning_RGB_color(80, 0, 80, MENU_WARNING_50); // Haki
          velocidadBase = 90;
          velocidadMsBase = 4.50;
          break;
        case 17:
          warning_RGB_color(80, 0, 80, MENU_WARNING_75); // Haki
          velocidadBase = 95;
          velocidadMsBase = 4.75;
          break;
        case 18:
          set_RGB_color(80, 80, 80); // Gear 5
          velocidadBase = 100;
          velocidadMsBase = 5.0;
          break;
      }
      break;
    case MODE_FANS:
      switch (valorConfig[modoConfig]) {
        case 0:
          set_RGB_color(0, 10, 10); // Cian
          velocidadVentiladorBase = 0;
          break;
        case 1:
          warning_RGB_color(0, 10, 10, MENU_WARNING_50); // Cian
          velocidadVentiladorBase = 10;
          break;
        case 2:
          set_RGB_color(0, 80, 0); // Verde
          velocidadVentiladorBase = 20;
          break;
        case 3:
          warning_RGB_color(0, 80, 0, MENU_WARNING_50); // Verde
          velocidadVentiladorBase = 30;
          break;
        case 4:
          set_RGB_color(80, 80, 0); // Amarillo
          velocidadVentiladorBase = 40;
          break;
        case 5:
          warning_RGB_color(80, 80, 0, MENU_WARNING_50); // Amarillo
          velocidadVentiladorBase = 50;
          break;
        case 6:
          set_RGB_color(80, 0, 0); // Rojo
          velocidadVentiladorBase = 60;
          break;
        case 7:
          warning_RGB_color(80, 0, 0, MENU_WARNING_50); // Rojo
          velocidadVentiladorBase = 70;
          break;
        case 8:
          set_RGB_color(80, 0, 80); // Haki
          velocidadVentiladorBase = 80;
          break;
        case 9:
          warning_RGB_color(80, 0, 80, MENU_WARNING_50); // Haki
          velocidadVentiladorBase = 90;
          break;
        case 10:
          warning_RGB_color(80, 80, 80, MENU_WARNING_50); // Gear 5
          velocidadVentiladorBase = 100;
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

static uint8_t get_num_modos(void) {
  if (get_config_run() == CONFIG_RUN_RACE) {
    return NUM_MODOS_RACE; // NOTHING - VELOCIDAD - VENTILADORES
  } else {
    return NUM_MODOS_DEBUG; // NOTHING - VELOCIDAD - VENTILADORES - DEBUG
  }
}

static void refresh_menu_from_eeprom(void) {
  uint16_t *data = eeprom_get_data();
  for (uint8_t i = 0; i < NUM_MODOS_DEBUG; i++) {
    uint8_t numValores = 0;
    switch (i) {
      case MODE_SPEED:
        numValores = NUM_VALORES_SPEED;
        break;
      case MODE_FANS:
        numValores = NUM_VALORES_FANS;
        break;
      case MODE_DEBUG:
        numValores = NUM_VALORES;
        break;
    }
    if (data[DATA_INDEX_MENU + i] < numValores) {
      valorConfig[i] = data[DATA_INDEX_MENU + i];
    } else {
      valorConfig[i] = 0;
    }
  }
}

void check_menu_button(void) {
  refresh_menu_from_eeprom();
  handle_menu_mode();
  handle_menu_value();

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
      case MODE_SPEED:
        if (valorConfig[modoConfig] >= NUM_VALORES_SPEED) {
          valorConfig[modoConfig] = 0;
        }
        break;
      case MODE_FANS:
        if (valorConfig[modoConfig] >= NUM_VALORES_FANS) {
          valorConfig[modoConfig] = 0;
        }
        break;
      default:
        if (valorConfig[modoConfig] >= NUM_VALORES) {
          valorConfig[modoConfig] = 0;
        }
        break;
    }
    while (get_menu_up_btn()) {
      handle_menu_value();
      handle_menu_mode();
    };
    eeprom_set_data(DATA_INDEX_MENU, valorConfig, NUM_MODOS_DEBUG);
    delay(50);
  }

  // Comprueba descenso de valor de configuración
  if (get_menu_down_btn()) {
    if (valorConfig[modoConfig] > 0) {
      valorConfig[modoConfig]--;
    }
    while (get_menu_down_btn()) {
      handle_menu_value();
      handle_menu_mode();
    };
    eeprom_set_data(DATA_INDEX_MENU, valorConfig, NUM_MODOS_DEBUG);
    delay(50);
  }
}

uint8_t get_base_speed(void) {
  return velocidadBase;
}

float get_base_ms_speed(void) {
  if (get_config_track() == CONFIG_TRACK_LINEFOLLOWER) {
    return velocidadMsBase;
  } else {
    return velocidadRobotracerMsBase;
  }
}

float get_robotracer_straight_ms_speed(void) {
  return velocidadRobotracerMsStraight;
}
float get_base_turn_acceleration_mss(void) {
  if (get_config_track() == CONFIG_TRACK_LINEFOLLOWER) {
    return 0;
  } else {
    return aceleracionCurvaRobotracerMss;
  }
}

float get_base_deceleration_mss(void) {
  if (get_config_track() == CONFIG_TRACK_LINEFOLLOWER) {
    return MAX_BREAK_MS2;
  } else {
    return deceleracionRobotracerMss;
  }
}

float get_base_acceleration_mss(void) {
  if (get_config_track() == CONFIG_TRACK_LINEFOLLOWER) {
    return MAX_ACCEL_MS2;
  } else {
    return aceleracionRobotracerMss;
  }
}

uint8_t get_base_fan_speed(void) {
  return velocidadVentiladorBase;
}

bool in_debug_mode(void) {
  return modoConfig == MODE_DEBUG;
}

void reset_menu_mode(void) {
  modoConfig = MODE_NOTHING;
}
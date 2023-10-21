#ifndef __EEPROM_H
#define __EEPROM_H

#include <leds.h>
#include <libopencm3/stm32/flash.h>
#include <menu.h>
#include <sensors.h>

#define EEPROM_BASE_ADDRESS (0x080E0000)

#define DATA_LENGTH ((NUM_SENSORES_MAX * 3) + (NUM_MODOS_RACE - 1))

#define DATA_INDEX_SENSORS_MAX 0
#define DATA_INDEX_SENSORS_MIN (DATA_INDEX_SENSORS_MAX + NUM_SENSORES_MAX)
#define DATA_INDEX_SENSORS_UMB (DATA_INDEX_SENSORS_MIN + NUM_SENSORES_MAX)
#define DATA_INDEX_MENU_SPEED (DATA_INDEX_SENSORS_UMB + 1)
#define DATA_INDEX_MENU_FANS (DATA_INDEX_MENU_SPEED + 1)

void eeprom_save(void);
void eeprom_load(void);
void eeprom_clear(void);
void eeprom_set_data(uint16_t index, uint16_t *data, uint16_t length);
uint16_t *eeprom_get_data(void);

#endif

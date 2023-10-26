#include "eeprom.h"

static uint16_t eeprom_data[DATA_LENGTH];
static uint32_t millis_save = 0;

void eeprom_save(void) {
  uint32_t addr = EEPROM_BASE_ADDRESS;
  millis_save = get_clock_ticks();
  while(get_clock_ticks() - millis_save < 500){
    warning_status_led(50);
  }
  set_status_led(true);
  flash_unlock();
  flash_erase_sector(EEPROM_SECTOR, FLASH_CR_PROGRAM_X16);
  for (uint16_t i = 0; i < DATA_LENGTH; i++) {
    flash_program_word(addr, eeprom_data[i]);
    addr += 4;
  }
  flash_lock();
  set_status_led(false);
}

void eeprom_load(void) {
  uint32_t addr = EEPROM_BASE_ADDRESS;
  for (uint16_t i = 0; i < DATA_LENGTH; i++) {
    eeprom_data[i] = MMIO32(addr);
    addr += 4;
  }
}

void eeprom_clear(void) {
  flash_unlock();
  flash_erase_sector(EEPROM_SECTOR, FLASH_CR_PROGRAM_X16);
  flash_lock();
}

void eeprom_set_data(uint16_t index, uint16_t *data, uint16_t length) {
  for (uint16_t i = index; i < index + length; i++) {
    eeprom_data[i] = data[i - index];
  }
}

uint16_t *eeprom_get_data(void){
  return eeprom_data;
}
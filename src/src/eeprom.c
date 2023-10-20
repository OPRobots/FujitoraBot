#include "eeprom.h"

void write_to_eeprom(uint32_t *data, uint16_t size) {
  uint32_t addr = EEPROM_BASE_ADDRESS;

  // Unlock the Flash Program memory
  flash_unlock();

  // Erase the sector before writing
  flash_erase_sector(11, FLASH_CR_PROGRAM_X32);

  // Write the data to the EEPROM
  for (uint16_t i = 0; i < size; i++) {
    flash_program_word(addr, data[i]);
    addr += 4; // Move to the next word (4 bytes)
  }

  // Lock the Flash Program memory
  flash_lock();
}

void read_from_eeprom(uint32_t *data, uint16_t size) {
  uint32_t addr = EEPROM_BASE_ADDRESS;

  // Read the data from the EEPROM
  for (uint16_t i = 0; i < size; i++) {
    data[i] = MMIO32(addr); 
    addr += 4; // Move to the next word (4 bytes)
  }
}

void erase_eeprom(void) {
  // Unlock the Flash Program memory
  flash_unlock();

  // Erase the sector before writing
  flash_erase_sector(11, FLASH_CR_PROGRAM_X32);

  // Lock the Flash Program memory
  flash_lock();
}

#ifndef __EEPROM_H
#define __EEPROM_H

#include <libopencm3/stm32/flash.h>
#include <leds.h>

#define RESULT_OK 0
#define FLASH_WRONG_DATA_WRITTEN 0x80

#define EEPROM_BASE_ADDRESS (0x080E0000)

void write_to_eeprom(uint32_t* data, uint16_t size);
void read_from_eeprom(uint32_t* data, uint16_t size);
void erase_eeprom(void);

#endif

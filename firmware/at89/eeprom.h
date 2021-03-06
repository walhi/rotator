#ifndef EEPROM_H
#define EEPROM_H

#include <stdint.h>

uint8_t readConfig(void);

void writeConfig(void);

void readAnt(void);

void writeAnt(void);

uint8_t getData(uint8_t* buf, uint8_t size, uint8_t addr);

#endif

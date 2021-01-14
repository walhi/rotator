#ifndef LCD_HW_H
#define LCD_HW_H

#include <stdint.h>

void LCDHwInit(void);

void LCDWrite(uint8_t data, uint8_t mode);


#endif

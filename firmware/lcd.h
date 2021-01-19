#ifndef LCD_H
#define LCD_H

#include <stdint.h>

void LCDInit(void);

void LCDWrite(uint8_t data, uint8_t mode);

void LCDPosition(uint8_t col, uint8_t row);

void LCDClear(void);

void LCDPrintString(char* str);

void LCDPrintf (const char * format, ...);

void LCDPrintChar(char c);

#endif

#ifndef LCD_H
#define LCD_H

#include <stdint.h>

#ifdef __SDCC
#define LCDPrintf printf_fast
#else
void LCDPrintf (const char * format, ...);
#endif

void LCDInit(void);

void LCDWrite(uint8_t data, uint8_t mode);

void LCDPosition(uint8_t col, uint8_t row);

void LCDClear(void);

void LCDPrintString(char* str);

void LCDPrintChar(char c);

void LCDPrintBigDigit(uint8_t digit, uint8_t pos);

#endif

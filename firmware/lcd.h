#ifndef LCD_H
#define LCD_H

#include <stdint.h>
#include "lcd_hw.h"

inline void LCDReverse(void){
	LCDWrite(0x04, 0); /* Вывод в обратном порядке */
}

inline void LCDNormal(void){
	LCDWrite(0x06, 0); /* Вывод в обратном порядке */
}

void LCDPrint (int16_t value);

void LCDPrintf (const char * format, int16_t value);

void LCDInit(void);

void LCDWrite(uint8_t data, uint8_t mode);

void LCDPosition(uint8_t col, uint8_t row);

void LCDClear(void);

void LCDPrintString(char* str);

inline void LCDPrintChar(char c){
  LCDWrite(c, 1);
}

#define BIG_FONT
#ifdef BIG_FONT
void LCDPrintBigDigit(uint8_t digit, uint8_t pos);
#endif

#endif

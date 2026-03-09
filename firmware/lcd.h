#ifndef LCD_H
#define LCD_H

#include <stdint.h>
#include "lcd_hw.h"


void LCDInit(void);


inline void LCDReverse(void){
	LCDWrite(0x04, 0); /* Вывод в обратном порядке */
}

inline void LCDNormal(void){
	LCDWrite(0x06, 0); /* Вывод в обратном порядке */
}

void LCDPrint(uint16_t value, uint8_t length);

void LCDWriteBuf(const uint8_t* buf, uint8_t count);

void LCDPosition(uint8_t col, uint8_t row);

void LCDClear(void);

void LCDPrintString(char* str);

void LCDWriteBuf(const uint8_t* buf, uint8_t count);


inline void LCDPrintChar(char c){
  LCDWrite(c, 1);
}


//#define BIG_FONT
#ifdef BIG_FONT
void LCDPrintBigDigit(uint8_t digit, uint8_t pos);
#endif

#endif

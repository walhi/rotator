#include "lcd.h"

#include <8052.h>
#include <stdint.h>

#define LCD_D7 P2_7
#define LCD_D6 P2_6
#define LCD_D5 P2_5
#define LCD_D4 P2_4
#define LCD_E  P2_3
#define LCD_RW P2_2
#define LCD_RS P2_1


void LCDWrite(uint8_t data, uint8_t mode)
{
  LCD_RS = mode;
  LCD_RW = 0;
#ifdef LCD_D0
  // 8 bit mode
#else
  /* 4 bit mode */
  LCD_D7 = data & 0x80;
  LCD_D6 = data & 0x40;
  LCD_D5 = data & 0x20;
  LCD_D4 = data & 0x10;
  LCD_E  = 0;
  LCD_E  = 1;
  LCD_D7 = data & 0x08;
  LCD_D6 = data & 0x04;
  LCD_D5 = data & 0x02;
  LCD_D4 = data & 0x01;
  LCD_E  = 0;
  LCD_E  = 1;
#endif
}

void LCDInit()
{
  P2 = 0xff;
  LCD_E  = 1;
  LCDWrite(0x20, 0);
  LCDWrite(0x20, 0);
  LCDWrite(0xf0, 0);
}

void LCDClear();
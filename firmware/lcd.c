#include <stdarg.h>
#include <stdio.h>
#include <util/delay.h>
#include "lcd.h"
#include "lcd_hw.h"

#define LCD_CMD_HOME       0b10000000  /* Cursor home*/
#define LCD_CMD_BLINK_ON   0b00001111  /* Cursor Blink    */
#define LCD_CMD_BLINK_OFF  0b00001110  /* Cursor No Blink */
#define LCD_CMD_CLR        0b00000001  /* Clear LCD */


char LCDString[17];

void LCDInit()
{
  LCDHwInit();
  LCDWrite(0x02, 0);                /* Configure the LCD in 4-bit mode, */
  _delay_ms(5);
  LCDWrite(0x28, 0);                /* 2 line and 5x7 font */
  _delay_ms(5);
  LCDWrite(0x0C, 0);                /* Display On and Cursor On */
  _delay_ms(5);
	LCDWrite(0x06, 0);                /* Increment cursor */
  _delay_ms(5);
  LCDWrite(0x80, 0);                /* Set cursor position to 1st line, 1st column */
  _delay_ms(5);
}

void LCDPosition(uint8_t col, uint8_t row)
{
  if (row == 1) col += 0xC0;
  col |= 0x80;
  LCDWrite(col, 0);
  _delay_ms(5);
}

void LCDClear(){
  LCDWrite(LCD_CMD_CLR, 0);
}

int LCDPrintf (const char * format, ...)
{
	va_list ap;
	va_start(ap, format);
	int result = vsprintf(LCDString, format, ap);
	va_end(ap);
	LCDPrintString(LCDString);
}

void LCDPrintString(char* str)
{
  uint8_t i;
  for (i = 0; i < 16; i++){
    if (str[i] == 0) break;
    LCDWrite(str[i], 1);
  }
}

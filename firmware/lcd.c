#include <stdarg.h>
#include <stdio.h>
#include "lcd.h"
#include "lcd_hw.h"
#include "delay_hw.h"

#ifdef __SDCC /* SDCC */
#define CODE_MEM_BEFORE __code
#define CODE_MEM_AFTER
#else /* AVR GCC */
//#include <avr/pgmspace.h>
#define CODE_MEM_BEFORE
#define CODE_MEM_AFTER
/* #define CODE_MEM_AFTER PROGMEM */
#endif


/* Команды для LCD дисплея */
#define LCD_CMD_HOME       0b10000000  /* Cursor home*/
#define LCD_CMD_BLINK_ON   0b00001111  /* Cursor Blink    */
#define LCD_CMD_BLINK_OFF  0b00001110  /* Cursor No Blink */
#define LCD_CMD_CLR        0b00000001  /* Clear LCD */

/* Кастомные символы для большого шрифта */
/* https://github.com/varind/LCDCustomLargeFont */
CODE_MEM_BEFORE const uint8_t LT[8]  CODE_MEM_AFTER = {0x07, 0x0f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f};
CODE_MEM_BEFORE const uint8_t UB[8]  CODE_MEM_AFTER = {0x1f, 0x1f, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00};
CODE_MEM_BEFORE const uint8_t RT[8]  CODE_MEM_AFTER = {0x1c, 0x1e, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f};
CODE_MEM_BEFORE const uint8_t LL[8]  CODE_MEM_AFTER = {0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x0f, 0x07};
CODE_MEM_BEFORE const uint8_t LB[8]  CODE_MEM_AFTER = {0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x1f, 0x1f};
CODE_MEM_BEFORE const uint8_t LR[8]  CODE_MEM_AFTER = {0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1e, 0x1c};
CODE_MEM_BEFORE const uint8_t UMB[8] CODE_MEM_AFTER = {0x1f, 0x1f, 0x1f, 0x00, 0x00, 0x00, 0x1f, 0x1f};
CODE_MEM_BEFORE const uint8_t LMB[8] CODE_MEM_AFTER = {0x1f, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x1f, 0x1f};

CODE_MEM_BEFORE const uint8_t bigDigits[10][6] CODE_MEM_AFTER =	{
												 {7, 0, 1, 2, 3, 4},
												 {0, 1, ' ', 3, 255, 3},
												 {5, 5, 1, 2, 6, 6},
												 {5, 5, 1, 6, 6, 4},
												 {2, 3, 1, ' ', ' ', 255},
												 {255, 5, 5, 6, 6, 4},
												 {7, 5, 5, 2, 6, 4},
												 {0, 0, 1, ' ', 7, ' '},
												 {7, 5, 1, 2, 6, 4},
												 {7, 5, 1, ' ', ' ', 255}
};

#ifdef __SDCC
/*================================================*/
int putchar(int data){
  LCDWrite(data, 1);
  return data;
}
#else
/*================================================*/
int putchar2(int data){
  LCDWrite(data, 1);
  return data;
}
FILE lcd_str = FDEV_SETUP_STREAM(putchar2, NULL, _FDEV_SETUP_RW);

void LCDPrintf (const char * format, ...)
{
	va_list ap;
	va_start(ap, format);
	int result = vfprintf(&lcd_str, format, ap);
	va_end(ap);
}

#endif
/*================================================*/

void LCDWriteBuf(const uint8_t* buf, uint8_t count){
	uint8_t i;
	for (i = 0; i < count; i++){
		LCDWrite(buf[i], 1);
	}
}

void LCDInit()
{
  LCDHwInit();
  LCDWrite(0x02, 0);                /* Configure the LCD in 4-bit mode, */
  delay_hw_ms(5);
  LCDWrite(0x28, 0);                /* 2 line and 5x7 font */
  delay_hw_ms(5);
  LCDWrite(0x0C, 0);                /* Display On and Cursor On */
  delay_hw_ms(5);
	LCDWrite(0x06, 0);                /* Increment cursor */
  delay_hw_ms(5);
  LCDWrite(0x80, 0);                /* Set cursor position to 1st line, 1st column */
  delay_hw_ms(5);

	/* Загрузка кастомных символов для больших шрифтов */

  LCDWrite(0x40, 0); /* Set CGRAM address 0 */
	LCDWriteBuf(UB, 8);
	LCDWriteBuf(RT, 8);
	LCDWriteBuf(LL, 8);
	LCDWriteBuf(LB, 8);
	LCDWriteBuf(LR, 8);
	LCDWriteBuf(UMB, 8);
	LCDWriteBuf(LMB, 8);
	LCDWriteBuf(LT, 8);

}

void LCDPosition(uint8_t col, uint8_t row)
{
  if (row == 1) col += 0xC0;
  col |= 0x80;
  LCDWrite(col, 0);
  delay_hw_ms(5);
}

void LCDClear(){
  LCDWrite(LCD_CMD_CLR, 0);
}

void LCDPrintString(char* str)
{
  uint8_t i;
  for (i = 0; i < 16; i++){
    if (str[i] == 0) break;
    LCDWrite(str[i], 1);
  }
}

void LCDPrintChar(char c)
{
  LCDWrite(c, 1);
}

void LCDPrintBigDigit(uint8_t digit, uint8_t pos)
{
	if (digit > 9) return;
	LCDPosition(pos, 0);
	LCDWriteBuf(&(bigDigits[digit][0]), 3);
	LCDPosition(pos, 1);
	LCDWriteBuf(&(bigDigits[digit][3]), 3);

}

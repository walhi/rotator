#include <8052.h>
#include "lcd_hw.h"
#include "delay_hw.h"

#define LCD_RS_PIN P2_1
#define LCD_RW_PIN P2_2
#define LCD_E_PIN  P2_3
#define LCD_D4_PIN P2_4
#define LCD_D5_PIN P2_5
#define LCD_D6_PIN P2_6
#define LCD_D7_PIN P2_7

void LCDHwInit(){
  P2 = 0xff;
}

void LCDWrite(uint8_t data, uint8_t mode)
{
  LCD_RS_PIN = mode;
	LCD_RW_PIN = 0;
  /* 4 bit mode */
  LCD_D7_PIN = (data & 0x80);
  LCD_D6_PIN = (data & 0x40);
  LCD_D5_PIN = (data & 0x20);
  LCD_D4_PIN = (data & 0x10);
  LCD_E_PIN = 0;
  delay_hw_us(40); /* TODO */
  LCD_E_PIN = 1;
  LCD_D7_PIN = (data & 0x08);
  LCD_D6_PIN = (data & 0x04);
  LCD_D5_PIN = (data & 0x02);
  LCD_D4_PIN = (data & 0x01);

  LCD_E_PIN = 0;
  delay_hw_us(40); /* TODO */
  LCD_E_PIN = 1;
}

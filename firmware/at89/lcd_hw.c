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

/*
__at(0x20) uint8_t data_b;
__sbit __at (0x0) DATA_B0;
__sbit __at (0x1) DATA_B1;
__sbit __at (0x2) DATA_B2;
__sbit __at (0x3) DATA_B3;
__sbit __at (0x4) DATA_B4;
__sbit __at (0x5) DATA_B5;
__sbit __at (0x6) DATA_B6;
__sbit __at (0x7) DATA_B7;
*/

#define E_PULSE_WIDTH 40

void LCDHwInit(void){
  P2 = 0xff;

	LCD_RS_PIN = 1;
	LCD_RW_PIN = 1;
	LCD_E_PIN = 1;
}

void LCDCommand4(uint8_t data)
{
	data = data & 0x0f;
	//data_b = data;
  LCD_RS_PIN = 0;
	LCD_RW_PIN = 0;
  /* 4 bit mode */
  //LCD_D7_PIN = DATA_B3;
  //LCD_D6_PIN = DATA_B2;
  //LCD_D5_PIN = DATA_B1;
  //LCD_D4_PIN = DATA_B0;
  LCD_D7_PIN = (data & 0x8);
  LCD_D6_PIN = (data & 0x4);
  LCD_D5_PIN = (data & 0x2);
  LCD_D4_PIN = (data & 0x1);
  LCD_E_PIN = 0;
  delay_hw_us(E_PULSE_WIDTH); /* TODO */
  LCD_E_PIN = 1;
  delay_hw_ms(15);
}

void LCDWrite(uint8_t data, uint8_t mode)
{
	//data_b = data;
  LCD_RS_PIN = mode;
	LCD_RW_PIN = 0;
  /* 4 bit mode */
  //LCD_D7_PIN = DATA_B7;
  //LCD_D6_PIN = DATA_B6;
  //LCD_D5_PIN = DATA_B5;
  //LCD_D4_PIN = DATA_B4;
  LCD_D7_PIN = (data & 0x80);
  LCD_D6_PIN = (data & 0x40);
  LCD_D5_PIN = (data & 0x20);
  LCD_D4_PIN = (data & 0x10);
  LCD_E_PIN = 0;
  delay_hw_us(E_PULSE_WIDTH); /* TODO */
  LCD_E_PIN = 1;
  //LCD_D7_PIN = DATA_B3;
  //LCD_D6_PIN = DATA_B2;
  //LCD_D5_PIN = DATA_B1;
  //LCD_D4_PIN = DATA_B0;
  LCD_D7_PIN = (data & 0x08);
  LCD_D6_PIN = (data & 0x04);
  LCD_D5_PIN = (data & 0x02);
  LCD_D4_PIN = (data & 0x01);

  LCD_E_PIN = 0;
  delay_hw_us(E_PULSE_WIDTH); /* TODO */
  LCD_E_PIN = 1;
}

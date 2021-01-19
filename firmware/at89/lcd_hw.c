#include <8052.h>
#include "lcd_hw.h"
#include "delay_hw.h"


#define PIN_WRITE(x, y) {x = (y)?1:0;}


#define LCD_RS_PORT_REG P2
#define LCD_RW_PORT_REG P2
#define LCD_E_PORT_REG P2
#define LCD_D4_PORT_REG P2
#define LCD_D5_PORT_REG P2
#define LCD_D6_PORT_REG P2
#define LCD_D7_PORT_REG P2

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
  PIN_WRITE(LCD_RS_PIN, mode);
  PIN_WRITE(LCD_RW_PIN, 0);
  /* 4 bit mode */
  PIN_WRITE(LCD_D7_PIN, (data & 0x80));
  PIN_WRITE(LCD_D6_PIN, (data & 0x40));
  PIN_WRITE(LCD_D5_PIN, (data & 0x20));
  PIN_WRITE(LCD_D4_PIN, (data & 0x10));
  PIN_WRITE(LCD_E_PIN, 0);
  delay_hw_us(100);
  PIN_WRITE(LCD_E_PIN, 1);
  PIN_WRITE(LCD_D7_PIN, (data & 0x08));
  PIN_WRITE(LCD_D6_PIN, (data & 0x04));
  PIN_WRITE(LCD_D5_PIN, (data & 0x02));
  PIN_WRITE(LCD_D4_PIN, (data & 0x01));

  PIN_WRITE(LCD_E_PIN, 0);
  delay_hw_us(100);
  PIN_WRITE(LCD_E_PIN, 1);
}

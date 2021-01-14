#include <util/delay.h>
#include "lcd_hw.h"
#include "signals.h"


#define PIN_WRITE(x, y, z) {if (z) x |= y; else x &= ~(y);}

void LCDHwInit(){
  LCD_RS_DDR_REG |= LCD_RS_PIN;
  LCD_RW_DDR_REG |= LCD_RW_PIN;
  LCD_E_DDR_REG  |= LCD_E_PIN;
  LCD_D4_DDR_REG |= LCD_D4_PIN;
  LCD_D5_DDR_REG |= LCD_D5_PIN;
  LCD_D6_DDR_REG |= LCD_D6_PIN;
  LCD_D7_DDR_REG |= LCD_D7_PIN;
}

void LCDWrite(uint8_t data, uint8_t mode)
{
  PIN_WRITE(LCD_RS_PORT_REG, LCD_RS_PIN, mode);
  PIN_WRITE(LCD_RW_PORT_REG, LCD_RW_PIN, 0);
#ifdef LCD_D0_PIN
  // 8 bit mode
  PIN_WRITE(LCD_D7_PORT_REG, LCD_D7_PIN, (data & 0x80));
  PIN_WRITE(LCD_D6_PORT_REG, LCD_D6_PIN, (data & 0x40));
  PIN_WRITE(LCD_D5_PORT_REG, LCD_D5_PIN, (data & 0x20));
  PIN_WRITE(LCD_D4_PORT_REG, LCD_D4_PIN, (data & 0x10));
  PIN_WRITE(LCD_D3_PORT_REG, LCD_D3_PIN, (data & 0x08));
  PIN_WRITE(LCD_D2_PORT_REG, LCD_D2_PIN, (data & 0x04));
  PIN_WRITE(LCD_D1_PORT_REG, LCD_D1_PIN, (data & 0x02));
  PIN_WRITE(LCD_D0_PORT_REG, LCD_D0_PIN, (data & 0x01));
#else
  /* 4 bit mode */
  PIN_WRITE(LCD_D7_PORT_REG, LCD_D7_PIN, (data & 0x80));
  PIN_WRITE(LCD_D6_PORT_REG, LCD_D6_PIN, (data & 0x40));
  PIN_WRITE(LCD_D5_PORT_REG, LCD_D5_PIN, (data & 0x20));
  PIN_WRITE(LCD_D4_PORT_REG, LCD_D4_PIN, (data & 0x10));
  PIN_WRITE(LCD_E_PORT_REG, LCD_E_PIN, 0);
  _delay_us(100);
  PIN_WRITE(LCD_E_PORT_REG, LCD_E_PIN, 1);
  PIN_WRITE(LCD_D7_PORT_REG, LCD_D7_PIN, (data & 0x08));
  PIN_WRITE(LCD_D6_PORT_REG, LCD_D6_PIN, (data & 0x04));
  PIN_WRITE(LCD_D5_PORT_REG, LCD_D5_PIN, (data & 0x02));
  PIN_WRITE(LCD_D4_PORT_REG, LCD_D4_PIN, (data & 0x01));
#endif
  PIN_WRITE(LCD_E_PORT_REG, LCD_E_PIN, 0);
  _delay_us(100);
  PIN_WRITE(LCD_E_PORT_REG, LCD_E_PIN, 1);
}

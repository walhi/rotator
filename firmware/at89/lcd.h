#ifndef LCD_H
#define LCD_H


#define HOME       0b10000000  /* Cursor home*/
#define BLINK_ON   0b00001111  /* Cursor Blink    */
#define BLINK_OFF  0b00001110  /* Cursor No Blink */
#define CLR        0b00000001  /* Clear LCD */

void LCDInit();

void LCDClear();

#endif
#include <8052.h>
#include "lcd.h"
#include <stdio.h>

#define led P1_1


#define key hhh

#include <stdint.h>
uint8_t hhh = 0;

char LCDString[16];


#define LCD_D7 P2_7
#define LCD_D6 P2_6
#define LCD_D5 P2_5
#define LCD_D4 P2_4
#define LCD_E  P2_3
#define LCD_RW P2_2
#define LCD_RS P2_1

void LCDWrite4bit(uint8_t data, uint8_t mode)
{
  LCD_RS = mode;
  LCD_RW = 0;
  LCD_D7 = data & 0x8;
  LCD_D6 = data & 0x4;
  LCD_D5 = data & 0x2;
  LCD_D4 = data & 0x1;
  LCD_E  = 0;
  LCD_E  = 1;
}

void LCDWrite(uint8_t data, uint8_t mode)
{
  LCD_RS = mode;
  LCD_RW = 0;
#ifdef LCD_D0
  // 8 bit mode
  LCD_D7 = data & 0x80;
  LCD_D6 = data & 0x40;
  LCD_D5 = data & 0x20;
  LCD_D4 = data & 0x10;
  LCD_D3 = data & 0x08;
  LCD_D2 = data & 0x04;
  LCD_D1 = data & 0x02;
  LCD_D0 = data & 0x01;
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
#endif
  LCD_E  = 0;
  LCD_E  = 1;
}

void LCDInit()
{
  P2 = 0xff;
  LCD_E  = 1;
  LCDWrite(0x02, 0);                /* Configure the LCD in 4-bit mode, */
  LCDWrite(0x28, 0);                /* 2 line and 5x7 font */
  LCDWrite(0x0C, 0);                /* Display On and Cursor On */
	LCDWrite(0x06, 0);                /* Increment cursor */
  LCDWrite(0x80, 0);                /* Set cursor position to 1st line, 1st column */
}

void LCDPosition(uint8_t col, uint8_t row)
{
        if (row == 1) col += 0xC0;
        col |= 0x80;
        LCDWrite(col, 0);
}

void LCDClear(){
  LCDWrite(CLR, 0);
}

void LCDPrintString(char* str)
{
  uint8_t i;
  for (i = 0; i < 16; i++){
    if (str[i] == 0) break;
    LCDWrite(str[i], 1);
  }
}


void delay(int time)
{
	do {} while(time--);
}

void printHeader(){
  LCDClear();
	LCDPosition(0, 0);
	sprintf(LCDString, "PORT azimuth");
	LCDPrintString(LCDString);
	LCDPosition(0, 1);
	sprintf(LCDString, " ANT azimuth");
	LCDPrintString(LCDString);
}

uint16_t antAzimuth = 26;
uint16_t targetAzimuth = 0xffff;

void printTarget(){
	LCDPosition(13, 0);
	if (targetAzimuth < 360){ /* init value: 0xffff */
  	sprintf(LCDString, "%03d", targetAzimuth);
  	LCDPrintString(LCDString);
	} else {
    LCDWrite(' ', 1);
	}
}

void printAnt(){
	LCDPosition(13, 1);
	sprintf(LCDString, "%03d", antAzimuth);
	LCDPrintString(LCDString);
}

void printValues(){
  printTarget();
  printAnt();
}

int main()
{
  uint8_t oldKey = key;
	P1 = 0xff;
	P2 = 0xff;
	LCDInit();
	printHeader();
	printValues();
	
  IT0 = 1;       /* Configure INT0 falling edge interrupt */
  EX0 = 1;       /* Enable the INT0 External Interrupt */
  
  IT1 = 1;       /* Configure INT1 falling edge interrupt */
  EX1 = 1;       /* Enable the INT1 External Interrupt */
  
  EA  = 1;       /* Enable the Global Interrupt bit */

	
	while (1){
	  led = !led;
	  if (key != oldKey){
      oldKey = key;
      if (key == 0){
        /*antAzimuth++;*/
        key = 0xff;
        printAnt();
      }
	  }
		delay(2);
	}
}

/* Falling edge INTO */
void azimuthEncoder_ISR() __interrupt 0
{
	  key = !key;
	  antAzimuth++;
}

/* Falling edge INT1 */
void elevationEncoder_ISR() __interrupt 2
{
	  key = !key;
	  antAzimuth--;
}
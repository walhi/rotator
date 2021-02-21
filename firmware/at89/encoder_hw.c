#include <8052.h>
#include "encoder_hw.h"

void encoderHwInit()
{
  /* В 8051 порт 0 может быть использован */
  /* только как вход. Но для этого нужно  */
  /* записать единицу в любой из пинов    */
  /* К портам 1-3 это не относится        */
	P0 = 0xff;

	/* Инициализация таймера 0 */
	TMOD |= T0_M0; /* Mode 1 (16 bit) */
	ET0 = 1;  /* Enable interrupt */
	EA  = 1;  /* Enable global interrupt */
}


static uint8_t flag = 1;

void timerReload()
{
	TL0 = 0xff;
	TH0 = 0xff;
	TR0 = 1;
	flag = TIMER_TIME;
}


uint8_t timerStatus()
{
	return flag;
}

/* void Timer0_ISR() __interrupt TF0_VECTOR */
void timer0Int()
{
	flag--;
	if (flag == 0){
		TR0 = 0;
	}
}

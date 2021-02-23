#include <8051.h>
#include "clock.h"

static uint32_t mseconds = 0;

void clockInit()
{
	/* Инициализация таймера 0 */
	TMOD |= T0_M0; /* Mode 1 (16 bit) */
	TR0 = 1;
	TH0 = 0xff;        //Load the timer value
	TL0 = 0xff;
	ET0 = 1;  /* Enable interrupt */
	EA  = 1;  /* Enable global interrupt */
}

uint8_t timerCheck(struct timer* t)
{
	return (t->alarmTime < mseconds);
}

void timerReload(struct timer* t, uint16_t interval)
{
	t->alarmTime = mseconds + interval;
}

/* 30.5 переполнений таймера в секунду на частоте 24 МГц */
/* Одно переполнение соответствует ~33 мс*/
void timer0Int()
{
	TF0 = 0;
	mseconds += 33;
}

#include <8052.h>
#include "encoder_hw.h"

void encoderHwInit()
{
  /* В 8051 порт 0 может быть использован */
  /* только как вход. Но для этого нужно  */
  /* записать единицу в любой из пинов    */
  /* К портам 1-3 это не относится        */
	P0 = 0xff;

	/* Инициализация таймера 2 */
	RCAP2H = 0xfc;
	TR2 = 1;
	ET2 = 1;  /* Enable interrupt */
	EA  = 1;  /* Enable global interrupt */
}

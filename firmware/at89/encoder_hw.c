#include <8052.h>
#include "encoder_hw.h"

void encoderHwInit()
{
  /* В 8051 порт 0 может быть использован */
  /* только как вход. Но для этого нужно  */
  /* записать единицу в любой из пинов    */
  /* К портам 1-3 это не относится        */
	P0 = 0xff;
}

uint8_t encoderHwAzAGet()
{
	return P0_1;
}

uint8_t encoderHwAzBGet()
{
  return P0_0;
}

uint8_t encoderHwAzBtnGet()
{
  return P0_4;
}

uint8_t encoderHwElAGet()
{
  return P0_3;
}

uint8_t encoderHwElBGet()
{
  return P0_2;
}

uint8_t encoderHwElBtnGet()
{
  return P0_5;
}

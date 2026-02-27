#ifndef ENCODER_HW_H
#define ENCODER_HW_H

#include <stdint.h>
#include <8052.h>

#define TIMER_TIME 15

void encoderHwInit(void);

inline uint8_t encoderHwAzAGet(void)
{
	return P0_1;
}

inline uint8_t encoderHwAzBGet(void)
{
  return P0_0;
}

inline uint8_t encoderHwAzBtnGet(void)
{
  return P0_4;
}

inline uint8_t encoderHwElAGet(void)
{
  return P0_3;
}

inline uint8_t encoderHwElBGet(void)
{
  return P0_2;
}

inline uint8_t encoderHwElBtnGet(void)
{
  return P0_5;
}
#endif

#ifndef ENCODER_HW_H
#define ENCODER_HW_H

#include <stdint.h>
#include <8052.h>

#define TIMER_TIME 15

void encoderHwInit(void);

inline uint8_t encoderHwAzAGet()
{
	return P0_1;
}

inline uint8_t encoderHwAzBGet()
{
  return P0_0;
}

inline uint8_t encoderHwAzBtnGet()
{
  return P0_4;
}

inline uint8_t encoderHwElAGet()
{
  return P0_3;
}

inline uint8_t encoderHwElBGet()
{
  return P0_2;
}

inline uint8_t encoderHwElBtnGet()
{
  return P0_5;
}
#endif

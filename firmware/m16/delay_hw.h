#ifndef DELAY_HW_H
#define DELAY_HW_H

#include <stdint.h>
#include <util/delay.h>

inline static void delay_hw_ms(uint16_t value)
{
  _delay_ms(value);
}

inline static void delay_hw_us(uint16_t value)
{
  _delay_us(value);
}
#endif

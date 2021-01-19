#include "delay_hw.h"

void delay_hw_ms(uint16_t value)
{
  while(value){
    delay_hw_us(1000);
    value--;
  }
}

void delay_hw_us(uint16_t value)
{
  while (value){
    value--;
  }
}

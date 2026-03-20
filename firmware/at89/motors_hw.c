#include <8052.h>
#include "motors_hw.h"

void motorsHwInit(void)
{
  P1 |= 0xf0;
  P3 |= (1 << 6) | (1 << 7);
}

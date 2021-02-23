#include <8052.h>
#include "motors_hw.h"

void motorsHwInit()
{
  P1 |= 0x0f;
  P3 |= (1 << 6) | (1 << 7);
}

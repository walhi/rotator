#include <8052.h>
#include "motors_hw.h"

void motorsHwInit()
{
  P1 = 0xff;
  P3 |= (1 << 6);
}


/* ------------------------------------------------- */

void motorElEnable(void)
{
}

void motorElDisable(void)
{
}

void motorElR1Enable(void)
{
}

void motorElR1Disable(void)
{
}

void motorElR2Enable(void)
{
}

void motorElR2Disable(void)
{
}

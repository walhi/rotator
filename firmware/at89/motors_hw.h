#ifndef MOTORS_HW_H
#define MOTORS_HW_H

#include <8052.h>


void motorsHwInit(void);

/* ------------------------------------------------- */

inline void motorAzEnable(void)
{
  P3_6 = 1;
}

inline void motorAzDisable(void)
{
  P3_6 = 0;
}

inline void motorAzR1Enable(void)
{
  P1_4 = 1;
}

inline void motorAzR1Disable(void)
{
  P1_4 = 0;
}

inline void motorAzR2Enable(void)
{
  P1_5 = 1;
}

inline void motorAzR2Disable(void)
{
  P1_5 = 0;
}

/* ------------------------------------------------- */

void motorElEnable(void);

void motorElDisable(void);

void motorElR1Enable(void);

void motorElR1Disable(void);

void motorElR2Enable(void);

void motorElR2Disable(void);
#endif

#ifndef MOTORS_HW_H
#define MOTORS_HW_H

#include <8052.h>
#include <stdint.h>

void motorsHwInit(void);

/* ------------------------------------------------- */
/* Управление транзистором */
inline void motorAzEnable(void)
{
  P3_6 = 1;
}

inline void motorAzDisable(void)
{
  P3_6 = 0;
}

/* Реле 1 */
inline void motorAzR1Enable(void)
{
  P1_4 = 1;
}

inline void motorAzR1Disable(void)
{
  P1_4 = 0;
}

inline int8_t motorAzR1Status(void)
{
  return P1_4;
}

/* Реле 2 */
inline void motorAzR2Enable(void)
{
  P1_5 = 1;
}

inline void motorAzR2Disable(void)
{
  P1_5 = 0;
}

inline int8_t motorAzR2Status(void)
{
  return P1_5;
}

/* ------------------------------------------------- */

inline void motorElEnable(void)
{
	P3_7 = 1;
}

inline void motorElDisable(void)
{
	P3_7 = 0;
}

inline int8_t motorElR1Status(void)
{
	return P1_6;
}

inline void motorElR1Enable(void)
{
	P1_6 = 1;
}

inline void motorElR1Disable(void)
{
	P1_6 = 0;

}

inline int8_t  motorElR2Status(void)
{
	return P1_7;
}

inline void motorElR2Enable(void)
{
	P1_7 = 1;
}

inline void motorElR2Disable(void)
{
	P1_7 = 0;
}
#endif

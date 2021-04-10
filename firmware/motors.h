#ifndef MOTORS_H
#define MOTORS_H
#include "motors_hw.h"

void motorsInit(void);

inline int8_t motorAzTick(void)
{
	if (motorAzR1Status()) return -1;
	if (motorAzR2Status()) return 1;
	return 0;
}


void motorAzLeft(void);

void motorAzRight(void);

void motorAzStop(void);

inline int8_t motorElTick(void)
{
	if (motorElR1Status()) return 1;
	if (motorElR2Status()) return -1;
	return 0;
}

void motorElUp(void);

void motorElDown(void);

void motorElStop(void);

#endif

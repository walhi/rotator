#include <stdint.h>
#include "motors.h"
#include "motors_hw.h"

void motorsInit()
{
	motorsHwInit();
	motorAzDisable();
	motorAzR1Disable();
	motorAzR2Disable();
}

int8_t motorAzTick()
{
	if (motorAzR1Status()) return -1;
	if (motorAzR2Status()) return 1;
	return 0;
}

void motorAzLeft()
{
	motorAzR2Disable();
  motorAzR1Enable();
	motorAzEnable();
}

void motorAzRight()
{
	motorAzR1Disable();
  motorAzR2Enable();
	motorAzEnable();
}


void motorAzStop()
{
	motorAzDisable();
	motorAzR1Disable();
	motorAzR2Disable();
}


int8_t motorElTick()
{
	if (motorElR1Status()) return 1;
	if (motorElR2Status()) return -1;
	return 0;
}

void motorElUp()
{
  motorElR1Enable();
	motorElEnable();
}

void motorElDown()
{
  motorElR2Enable();
	motorElEnable();
}


void motorElStop()
{
	motorElDisable();
	motorElR1Disable();
	motorElR2Disable();
}

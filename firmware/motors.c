#include <stdint.h>
#include "motors.h"
#include "motors_hw.h"

void motorsInit(void)
{
	motorsHwInit();
	motorAzDisable();
	motorAzR1Disable();
	motorAzR2Disable();
}

void motorAzLeft(void)
{
	motorAzR2Disable();
  motorAzR1Enable();
	motorAzEnable();
}

void motorAzRight(void)
{
	motorAzR1Disable();
  motorAzR2Enable();
	motorAzEnable();
}

void motorAzStop(void)
{
	motorAzDisable();
	motorAzR1Disable();
	motorAzR2Disable();
}

void motorElUp(void)
{
  motorElR1Enable();
	motorElEnable();
}

void motorElDown(void)
{
  motorElR2Enable();
	motorElEnable();
}


void motorElStop(void)
{
	motorElDisable();
	motorElR1Disable();
	motorElR2Disable();
}

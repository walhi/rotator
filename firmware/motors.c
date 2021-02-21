#include <stdint.h>
#include "motors.h"
#include "motors_hw.h"

int8_t azimuthTick = 0;
int8_t elevationTick = 0;

void motorsInit()
{
	motorsHwInit();
}

void motorAzLeft()
{
  azimuthTick = -1;
  motorAzR1Enable();
	motorAzEnable();
}

void motorAzRight()
{
  azimuthTick = 1;
  motorAzR2Enable();
	motorAzEnable();
}


void motorAzStop()
{
  azimuthTick = 0;
	motorAzDisable();
	motorAzR1Disable();
	motorAzR2Disable();
}

void motorElUp()
{
  elevationTick = 1;
  motorElR1Enable();
	motorElEnable();
}

void motorElDown()
{
  elevationTick = -1;
  motorElR2Enable();
	motorElEnable();
}


void motorElStop()
{
  elevationTick = 0;
	motorElDisable();
	motorElR1Disable();
	motorElR2Disable();
}

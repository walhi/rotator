#include "motors.h"
#include "motors_hw.h"
#include "configure.h"

extern struct config cfg;
extern struct configFlags cfgFlags;
/*struct config cfg;*/

int8_t azimuthTick = 0;
int8_t elevationTick = 0;

void motorsInit()
{
	motorsHwInit();
}

void motorAzLeft()
{
  azimuthTick = -1;
	if (cfgFlags.az_motor_invert){
		motorAzR1Enable();
	} else {
		motorAzR2Enable();
	}
	motorAzEnable();
}

void motorAzRight()
{
  azimuthTick = 1;
	if (cfgFlags.az_motor_invert){
		motorAzR2Enable();
	} else {
		motorAzR1Enable();
	}
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
	if (cfgFlags.el_motor_invert){
		motorElR2Enable();
	} else {
		motorElR1Enable();
	}
	motorElEnable();
}

void motorElDown()
{
  elevationTick = -1;
	if (cfgFlags.el_motor_invert){
		motorElR2Enable();
	} else {
		motorElR1Enable();
	}
	motorElEnable();
}


void motorElStop()
{
  elevationTick = 0;
	motorElDisable();
	motorElR1Disable();
	motorElR2Disable();
}

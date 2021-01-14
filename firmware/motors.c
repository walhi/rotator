#include "motors.h"
#include "motors_hw.h"
#include "configure.h"

/*extern struct config cfg;*/
struct config cfg;


void motorsInit()
{
	motorsHwInit();
}

void motorAzLeft()
{
	if (cfg.az_motor_invert){
		motorAzR1Enable();
	} else {
		motorAzR2Enable();
	}
	motorAzEnable();
}

void motorAzRight()
{
	if (cfg.az_motor_invert){
		motorAzR2Enable();
	} else {
		motorAzR1Enable();
	}
	motorAzEnable();
}


void motorAzStop()
{
	motorAzDisable();
	motorAzR1Disable();
	motorAzR2Disable();
}

void motorElUp()
{
	if (cfg.az_motor_invert){
		motorElR2Enable();
	} else {
		motorElR1Enable();
	}
	motorElEnable();
}

void motorElDown()
{
	if (cfg.az_motor_invert){
		motorElR2Enable();
	} else {
		motorElR1Enable();
	}
	motorElEnable();
}


void motorElStop()
{
	motorElDisable();
	motorElR1Disable();
	motorElR2Disable();
}

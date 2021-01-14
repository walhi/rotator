#ifndef MOTORS_HW_H
#define MOTORS_HW_H

#include "signals.h"

void motorsHwInit(void);

/* ------------------------------------------------- */

inline void motorAzEnable(void)
{
	MOTOR_AZ_EN_PORT_REG |= MOTOR_AZ_EN_PIN;
}

inline void motorAzDisable(void)
{
	MOTOR_AZ_EN_PORT_REG &= ~(MOTOR_AZ_EN_PIN);
}

inline void motorAzR1Enable(void)
{
	MOTOR_AZ_R1_PORT_REG |= MOTOR_AZ_R1_PIN;
}

inline void motorAzR1Disable(void)
{
	MOTOR_AZ_R1_PORT_REG &= ~(MOTOR_AZ_R1_PIN);
}

inline void motorAzR2Enable(void)
{
	MOTOR_AZ_R2_PORT_REG |= MOTOR_AZ_R2_PIN;
}
inline void motorAzR2Disable(void)
{
	MOTOR_AZ_R2_PORT_REG &= ~(MOTOR_AZ_R2_PIN);
}

/* ------------------------------------------------- */

inline void motorElEnable(void)
{
	MOTOR_EL_EN_PORT_REG |= MOTOR_EL_EN_PIN;
}

inline void motorElDisable(void)
{
	MOTOR_EL_EN_PORT_REG &= ~(MOTOR_EL_EN_PIN);
}

inline void motorElR1Enable(void)
{
	MOTOR_EL_R1_PORT_REG |= MOTOR_EL_R1_PIN;
}

inline void motorElR1Disable(void)
{
	MOTOR_EL_R1_PORT_REG &= ~(MOTOR_EL_R1_PIN);
}

inline void motorElR2Enable(void)
{
	MOTOR_EL_R2_PORT_REG |= MOTOR_EL_R2_PIN;
}

inline void motorElR2Disable(void)
{
	MOTOR_EL_R2_PORT_REG &= ~(MOTOR_EL_R2_PIN);
}
#endif

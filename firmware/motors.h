#ifndef MOTORS_H
#define MOTORS_H

void motorsInit(void);

int8_t motorAzTick(void);

void motorAzLeft(void);

void motorAzRight(void);

void motorAzStop(void);

int8_t motorElTick(void);

void motorElUp(void);

void motorElDown(void);

void motorElStop(void);

#endif

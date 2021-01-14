#ifndef ENCODER_HW_H
#define ENCODER_HW_H

#include <stdint.h>

void encoderHwInit(void);

uint8_t encoderHwAzAGet(void);

uint8_t encoderHwAzBGet(void);

uint8_t encoderHwAzBtnGet(void);

uint8_t encoderHwElAGet(void);

uint8_t encoderHwElBGet(void);

uint8_t encoderHwElBtnGet(void);

#endif

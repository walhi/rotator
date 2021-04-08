#ifndef ENCODER_H
#define ENCODER_H

#include <stdint.h>

void encoderInit(void);

void encoderAzRead(void);

int8_t encoderAzGet(void);

int8_t encoderAzBtnGet(uint8_t cont);

void encoderElRead(void);

int8_t encoderElGet(void);

int8_t encoderElBtnGet(uint8_t cont);


#endif

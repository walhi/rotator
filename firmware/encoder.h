#ifndef ENCODER_H
#define ENCODER_H

#include <stdint.h>

void encoderInit(void);

int8_t encoderAzGet(void);

int8_t encoderAzBtnGet(uint8_t cont);

int8_t encoderElGet(void);

int8_t encoderElBtnGet(uint8_t cont);


#endif

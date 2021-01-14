#ifndef ENCODER_H
#define ENCODER_H

#include <stdint.h>

void encoderInit(void);

int8_t encoderAzGet(void);

int8_t encoderAzBtnGet(void);

int8_t encoderElGet(void);

int8_t encoderElBtnGet(void);


#endif

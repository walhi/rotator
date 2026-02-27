#ifndef ENCODER_H
#define ENCODER_H

#include <stdint.h>
#include "encoder_hw.h"

enum ButtonMode {
	BTN_SINGLE,
	BTN_CONT,
};

inline void encoderInit(void){
	  encoderHwInit();
}

void encoderAzRead(void);

int8_t encoderAzGet(void);

int8_t encoderAzBtnGet(enum ButtonMode mode);

void encoderElRead(void);

int8_t encoderElGet(void);

int8_t encoderElBtnGet(enum ButtonMode mode);


#endif

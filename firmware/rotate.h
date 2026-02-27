#ifndef ROTATE_H
#define ROTATE_H

#include <stdint.h>
#include "rotate_hw.h"

struct dir {
  unsigned right: 1;
  unsigned right_overlap: 1;
  unsigned left: 1;
  unsigned left_overlap: 1;
  unsigned up: 1;
  unsigned down: 1;
  unsigned wire_right: 1;
  unsigned wire_left: 1;
};

inline void rotateInit(void){
  rotateHwInit();
}

int16_t azD2P(int16_t value);

int16_t azP2D(int16_t value);

int16_t elD2P(int16_t value);

int16_t elP2D(int16_t value);

void azimuthImpulse(void);

void elevationImpulse(void);

#endif

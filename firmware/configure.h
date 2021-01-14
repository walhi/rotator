#ifndef CONFIGURE_H
#define CONFIGURE_H

#include <stdint.h>

struct config {
  int16_t az_count;
  int16_t az_overlap_start;
  int16_t az_overlap_end;
  int16_t el_enable;
  int16_t el_count;
  int16_t el_min;
  int16_t el_max;
  int16_t az_motor_invert;
  int16_t az_enc_invert;
  int16_t el_motor_invert;
  int16_t el_enc_invert;
  int16_t com_echo;
};

void configure(void);

#endif

#ifndef CONFIGURE_H
#define CONFIGURE_H

#include <stdint.h>


struct configAz {
  int16_t count;
  int16_t overlap_position;
  int16_t overlap_size;
  int16_t parking;
};

struct configEl {
  int16_t count;
  uint8_t min;
  uint8_t max;
  uint8_t parking;
};

struct configFlags {
  unsigned com_echo: 1;
  unsigned ui_use_old: 1;
  unsigned az_motor_invert: 1;
  unsigned az_enc_invert: 1;
  unsigned az_enc_int: 1;
  unsigned el_enable: 1;
  unsigned el_motor_invert: 1;
  unsigned el_enc_invert: 1;
  unsigned el_enc_int: 1;
};

void configure(void);

#endif

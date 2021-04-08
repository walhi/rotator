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
  unsigned ui_use_old: 1;
  unsigned el_enable: 1;
};

struct config {
	struct configAz Az;
  struct configEl El;
  struct configFlags Flags;
};

void configure(void);

#endif

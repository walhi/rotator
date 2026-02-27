#ifndef GS232_H
#define GS232_H

#include <stdint.h>
#include "ui.h"

/* Возвращает !0 при изменении целевого значения */
int8_t GS232Parse(enum workMode mode);

#endif

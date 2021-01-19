#include <stdint.h>
#include "rotate.h"
#include "rotate_hw.h"
#include "configure.h"


extern struct config cfg;
extern int8_t azimuthTick;
extern int8_t elevationTick;

int16_t antAzimuth = 139;
int16_t antElevation = 0;

static uint16_t antAzimuthFloat = 0;
static uint16_t antElevationFloat = 0;

void azimuthImpulse(){
  uint16_t tick = 36000 / cfg.az_count;
  antAzimuthFloat += azimuthTick * tick;
  antAzimuth = antAzimuthFloat / 100;
}

void elevationImpulse()
{

}

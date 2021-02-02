#include <stdint.h>
#include "rotate.h"
#include "rotate_hw.h"
#include "configure.h"

extern struct configAz cfgAz;
extern struct configEl cfgEl;
extern int8_t azimuthTick;
extern int8_t elevationTick;


int16_t antAzimuth = 140;
int16_t antElevation = 0;

static int16_t antAzimuthPos = 0; /* считать с eeprom количество тиков от 0 */
static int16_t antElevationPos = 0; /* считать с eeprom количество тиков от 0 */

void rotateInit(){
  rotateHwInit();
  antAzimuthPos = 0;
}

void azimuthImpulse(){
  if (azimuthTick){
    if (azimuthTick > 0)
      antAzimuthPos += 1;
    else
      antAzimuthPos -= 1;
  } else {
    /* Несанкционированное движение! */
  }
  if (antAzimuthPos >= cfgAz.count) antAzimuthPos = 0;
  if (antAzimuthPos < 0) antAzimuthPos = cfgAz.count;
  antAzimuth = ((360 << 4) / cfgAz.count * antAzimuthPos) >> 4; /* сдвиг для "увеличения точности"*/
}

void elevationImpulse()
{

}

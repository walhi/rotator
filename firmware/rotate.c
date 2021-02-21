#include <stdint.h>
#include "motors.h"
#include "rotate.h"
#include "rotate_hw.h"
#include "configure.h"

/* Конфигурация устройства */
extern struct configAz cfgAz;
extern struct configEl cfgEl;

/* Направление движения */
extern int8_t azimuthTick;
extern int8_t elevationTick;

/* Целевое значение положения */
extern int16_t targetAzimuthPos;
extern int16_t targetElevationPos;

/* Текущее положение антенны в "тиках" "энкодера" */
int16_t antAzimuthPos = 0;
int16_t antElevationPos = 0;

void rotateInit(){
  rotateHwInit();
}

void azimuthImpulse(){
  if (azimuthTick){
		antAzimuthPos += azimuthTick;
    if (antAzimuthPos >= cfgAz.count) antAzimuthPos -= cfgAz.count;
    if (antAzimuthPos < 0) antAzimuthPos += cfgAz.count;
    if (antAzimuthPos == targetAzimuthPos){
      /* Целевое положение достигнуто. Можно остановиться. */
      /* TODO что делать с движение по инерции? */
      motorAzStop();
    }
  } else {
    /* Несанкционированное движение! */
  }
}

void elevationImpulse()
{
  if (elevationTick){
		antElevationPos += elevationTick;
    if (antElevationPos >= cfgEl.count) antElevationPos -= cfgEl.count;
    if (antElevationPos < 0) antElevationPos += cfgEl.count;
    if (antElevationPos == targetElevationPos){
      /* Целевое положение достигнуто. Можно остановиться. */
      /* TODO что делать с движение по инерции? */
      motorElStop();
    }
  } else {
    /* Несанкционированное движение! */
  }
}

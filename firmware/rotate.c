#include <stdint.h>
#include "motors.h"
#include "rotate.h"
#include "rotate_hw.h"
#include "configure.h"

/* Конфигурация устройства */
extern struct config cfg;
//extern struct configAz cfgAz;
//extern struct configEl cfgEl;

/* Направление движения */
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
	int8_t tmp = motorAzTick();
  if (tmp){
		antAzimuthPos += tmp;
    if (antAzimuthPos >= cfg.Az.count) antAzimuthPos -= cfg.Az.count;
    if (antAzimuthPos < 0) antAzimuthPos += cfg.Az.count;
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
	uint8_t tick = motorElTick();
  if (tick){
		antElevationPos += tick;
    if (antElevationPos >= cfg.El.count) antElevationPos -= cfg.El.count;
    if (antElevationPos < 0) antElevationPos += cfg.El.count;
    if (antElevationPos == targetElevationPos){
      /* Целевое положение достигнуто. Можно остановиться. */
      /* TODO что делать с движение по инерции? */
      motorElStop();
    }
  } else {
    /* Несанкционированное движение! */
  }
}

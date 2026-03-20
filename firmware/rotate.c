#include <stdint.h>
#include "motors.h"
#include "rotate.h"
#include "rotate_hw.h"
#include "configure.h"

/* Конфигурация устройства */
extern struct config cfg;

/* Целевое значение положения */
int16_t targetAzimuthPos = 0;
int16_t targetElevationPos = 0;

/* Текущее положение антенны в "тиках" "энкодера" */
int16_t antAzimuthPos = 0;
int16_t antElevationPos = 0;

// Вещественных нет, так что создаем фиксированную точку
#define MUX 7 // Выбрана экспериментальным путем в excel.
#define SHIFT (1 << (MUX - 1)) // Константа на этапе компиляции

int16_t convert(int16_t scale1, int16_t scale2, int16_t value){
	int32_t res = scale1;
	res <<= MUX;
	res /= scale2;
	res *= value;
	res += SHIFT; // Для корректного "округления" путем отброса "дробной части"
	res >>= MUX;
	return res;

}

int16_t azD2P(int16_t value)
{
	return convert(cfg.Az.count, 360, value);
}

int16_t azP2D(int16_t value)
{
	return convert(360, cfg.Az.count, value);
}

int16_t elD2P(int16_t value)
{
	return convert(cfg.El.count, 180, value);
}

int16_t elP2D(int16_t value)
{
	return convert(180, cfg.El.count, value);
}

void azimuthImpulse(void){
	int8_t dir = motorAzTick();
  if (dir){
		antAzimuthPos += dir;
    if (antAzimuthPos >= cfg.Az.count)
			antAzimuthPos -= cfg.Az.count;
    else if (antAzimuthPos < 0)
			antAzimuthPos += cfg.Az.count;

    if (antAzimuthPos == targetAzimuthPos){
      /* Целевое положение достигнуто. Можно остановиться. */
      /* TODO что делать с движение по инерции? */
			/* Disable закрывает мосфет, но не отключает реле */
			/* Реле уберет основной цикл. Так отработает UI */
			/* и не будет коммутации реле под током */
			motorAzDisable();
      //motorAzStop();
    }
  } else {
    /* Несанкционированное движение! */
  }
}

void elevationImpulse(void)
{
	uint8_t dir = motorElTick();
  if (dir){
		antElevationPos += dir;
    if (antElevationPos >= cfg.El.count)
			antElevationPos -= cfg.El.count;
    else if (antElevationPos < 0)
			antElevationPos += cfg.El.count;

    if (antElevationPos == targetElevationPos){
      /* Целевое положение достигнуто. Можно остановиться. */
      /* TODO что делать с движение по инерции? */
			/* Disable закрывает мосфет, но не отключает реле */
			/* Реле уберет основной цикл. Так отработает UI */
			/* и не будет коммутации реле под током */
      motorElDisable();
      //motorElStop();
    }
  } else {
    /* Несанкционированное движение! */
  }
}

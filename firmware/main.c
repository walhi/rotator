#include <stdlib.h>

#include "lcd.h"
#include "motors.h"
#include "ui.h"
#include "configure.h"
#include "calibration.h"
#include "encoder.h"
#include "uart.h"
#include "gs232.h"
#include "delay_hw.h"
#include "rotate.h"
#include "rotate_hw.h"

/* Конфигурация */
extern struct configAz cfgAz;
extern struct configEl cfgEl;
extern struct configFlags cfgFlags;

/* Текущее положение */
extern int16_t antAzimuth;
extern int16_t antElevation;

/* Режим работы */
enum workMode mode;

/* Целевое значение */
int16_t targetAzimuth;
int16_t targetElevation;

/* Центр разрешенной для движения области */
int16_t virtualZero;

int main (void)
{
  int16_t virtualZero;
	int8_t step;
	uint8_t modeBtn;

	/* Задержка для включения LCD экрана. А нужна ли? */
  delay_hw_ms(50);

  /* Инициализация железа для работы с EEPROM */

	/* Чтение конфигурации из EEPROM */
  cfgAz.count = 360;
  cfgAz.overlap_position = 180;
  cfgAz.overlap_size = 10;
  cfgFlags.ui_use_old = 1;

	/* Инициализация остального железа */
  motorsInit();
  encoderInit();
  LCDInit();
	UARTInit();
  rotateInit();

	/* Чтение положения из EEPROM */
  targetAzimuth = antAzimuth;

	//configure();
  startupMessage();

	delay_hw_ms(300);

	/*=========================================*/
	/* Вход в режим калибровки азимута         */
	if (0 && encoderAzBtnGet() && !encoderElBtnGet()){
		calibrateAz();
	}

	/*=========================================*/
	/* Вход в режим калибровки элевации        */
	if (0 && cfgFlags.el_enable && !encoderAzBtnGet() && encoderElBtnGet()){
		calibrateEl();
	}

	/*=========================================*/
	/* Вход в режим настройки                  */
	if (0 && encoderAzBtnGet() && encoderElBtnGet()){
		/* configure(); */
	}

	/* Расчет виртуального нуля   .         */
	/* 180 - overlap pos                    */
	if (cfgAz.overlap_position > 180){
		virtualZero = cfgAz.overlap_position - 180;
	} else {
		virtualZero = cfgAz.overlap_position + 180;
	}
	virtualZero = (virtualZero > 180)?(virtualZero - 360):virtualZero;


	mode = port;
	/* startupMessage(); */
	initUI();
	printUI();


  while(1){
		/*=========================================*/
		/* Смена режимов работы                    */
		modeBtn = encoderElBtnGet();
    if (modeBtn){
      if (mode == port){
				mode = manual;
        targetAzimuth = antAzimuth;
			} else if (mode == manual){
				mode = port;
			}
    }

    /*=========================================*/
		/* Логика выбранного режима                */
		if (mode == port){
			if (UARTStatus()) GS232Parse();
    } else if (mode == manual){
			step = encoderAzGet();
      if (step){
        targetAzimuth += step;
        if (targetAzimuth >= 360) targetAzimuth -= 360;
        if (targetAzimuth < 0) targetAzimuth += 360;
      }
    }

		/*=========================================*/
		/* Интерфейс пользователя                  */
    printUI();

		continue;

		/*=========================================*/
		/* Логика поворота антенны                 */
		if (targetAzimuth != antAzimuth){
      int16_t tmpAntAzimuth;
      int16_t tmpTargetAzimuth;

      /* Переход от 0 - 360 в -180 - 180       */
      tmpAntAzimuth = (antAzimuth > 180)?(antAzimuth - 360):antAzimuth;
      tmpTargetAzimuth = (targetAzimuth > 180)?(targetAzimuth - 360):targetAzimuth;

			/* Поворот координат на виртуальный 0    */
			tmpAntAzimuth -= virtualZero;
      tmpTargetAzimuth -= virtualZero;

      /* Движение */
      if (tmpAntAzimuth < tmpTargetAzimuth){
        motorAzStop();
        motorAzRight();
      } else {
        motorAzStop();
        motorAzLeft();
      }
    } else {
      motorAzStop();
    }
  }

  return 0;
}

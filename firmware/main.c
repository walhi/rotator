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

/* Текущее положение антенны в градусах */
int16_t antAzimuth = 140;
int16_t antElevation = 0;

/* Текущее положение антенны в "тиках" "энкодера" */
extern int16_t antAzimuthPos;
extern int16_t antElevationPos;

/* Целевое значение положения антенны в градусах */
int16_t targetAzimuth;
int16_t targetElevation;

/* Целевое значение положения антенны в "тиках" "энкодера" */
int16_t targetAzimuthPos;
int16_t targetElevationPos;

/* Режим работы */
enum workMode mode;

/* Допустимые направления движения */
struct dir dirAllowed;

/* Переменные для обработки зоны overlap */
static int16_t virtualZero;
static int16_t tmpAntAzimuth;
static int16_t tmpTargetAzimuth;


/*=========================================*/
/* Расчет зоны overlap                     */
static void azConvert(void)
{
  /* Переход от 0 - 360 в -180 - 180       */
  tmpAntAzimuth = (antAzimuth > 180)?(antAzimuth - 360):antAzimuth;
  tmpTargetAzimuth = (targetAzimuth > 180)?(targetAzimuth - 360):targetAzimuth;
  /* Поворот координат на виртуальный 0    */
  tmpAntAzimuth -= virtualZero;
  tmpTargetAzimuth -= virtualZero;

  /* С какой стороны антенны провод */
  if (tmpAntAzimuth == 0){
    /* В виртуальном нуле, состояние провода */
    /* не определено                         */
    dirAllowed.wire_right = 0;
    dirAllowed.wire_left = 0;
  } else {
    if (dirAllowed.wire_right == 0 && dirAllowed.wire_left == 0){
      /* Начали движение от нуля, все просто */
      if (tmpAntAzimuth > 0)
        dirAllowed.wire_left = 1;
      else
        dirAllowed.wire_right = 1;
    } else {
      /* TODO                                     */
      /* Переход через ноль будет обязательно, но */
      /* Как задать стартовое значение?           */
      if (tmpAntAzimuth != 180 && tmpAntAzimuth != -180){
        if (tmpAntAzimuth < 0)
          dirAllowed.wire_right = 1;
        else
          dirAllowed.wire_left = 1;
      }
    }
  }
}


/*=========================================*/
/* Расчет допустимых направлений           */
static void calcDir(void)
{
	if (dirAllowed.wire_right){
		/* Провод расположен справа от антенны */
		/* В overlap зону можно заходить против часовой стрелки */
		dirAllowed.right_overlap = 0;
		dirAllowed.right = 1;
		if (tmpTargetAzimuth == (180 - cfgAz.overlap_size)){
			/* Дошли до края overlap зоны */
			dirAllowed.left = 0;
			dirAllowed.left_overlap = 1;
		} else if ((tmpTargetAzimuth < 0) && tmpTargetAzimuth > -180){
			/* Ещё не вошли в overlap зону */
			dirAllowed.left = 1;
			dirAllowed.left_overlap = 0;
		} else {
			/* Внутри overlap зоны */
			dirAllowed.left = 1;
			dirAllowed.left_overlap = 1;
		}
	} else if (dirAllowed.wire_left){
		/* Провод расположен слева от антенны */
		/* В overlap зону можно заходить по часовой стрелке */
		dirAllowed.left_overlap = 0;
		dirAllowed.left = 1;
		if (tmpTargetAzimuth == (-180 + cfgAz.overlap_size)){
			/* Дошли до края overlap зоны */
			dirAllowed.right = 0;
			dirAllowed.right_overlap = 0;
		} else if ((tmpTargetAzimuth > 0) && tmpTargetAzimuth < 180){
			/* Ещё не вошли в overlap зону */
			dirAllowed.right = 1;
			dirAllowed.right_overlap = 0;
		} else {
			/* Внутри overlap зоны */
			dirAllowed.right = 1;
			dirAllowed.right_overlap = 1;
		}
	} else {
		/* Антенна находится на виртуальном нуле */
		/* Разрешено использовать любое направление */
		dirAllowed.right = 1;
		dirAllowed.left = 1;
		dirAllowed.right_overlap = 0;
		dirAllowed.left_overlap = 0;
	}
}

extern int8_t azimuthTick;
int main (void)
{
	int8_t step;
	uint8_t modeBtn;

	/* Задержка для включения LCD экрана. А нужна ли? */
  delay_hw_ms(50);

  /* Инициализация железа для работы с EEPROM */

	/* Чтение конфигурации из EEPROM */
  //cfgAz.count = 576;
  cfgAz.count = 3600;
  cfgAz.overlap_position = 180;
  cfgAz.overlap_size = 10;
  cfgFlags.ui_use_old = 0;
	cfgFlags.com_echo = 0;

	/* Инициализация остального железа */
  LCDInit();
  motorsInit();
  encoderInit();
	UARTInit();
  rotateInit();

  /* Остановить все движение */
  motorAzStop();
  motorElStop();

	/* Чтение положения из EEPROM */
  targetAzimuth = antAzimuth;

  /* Расчет виртуального нуля             */
	/* 180 - overlap pos                    */
	if (cfgAz.overlap_position >= 180){
		virtualZero = cfgAz.overlap_position - 180;
	} else {
		virtualZero = cfgAz.overlap_position + 180;
	}
	virtualZero = (virtualZero > 180)?(virtualZero - 360):virtualZero;

  azConvert();

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


	calcDir();

	mode = port;


	initUI();
	printUI();

	int16_t loop_az;
	int16_t loop_target_az;
	uint8_t ggg, hhh;

  while(1){
		ggg = 0;
		hhh = 0;
		if (loop_az != antAzimuth){
			loop_az = antAzimuth;
			ggg = 1;
		}
		if (loop_target_az != targetAzimuth){
			loop_target_az = targetAzimuth;
			hhh = 1;
		}

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
		/* Обработка зоны overlap                  */
		azConvert();

		if (hhh){
			calcDir();
		}

    /*=========================================*/
		/* Логика выбранного режима                */
		if (mode == port){
			if (UARTStatus()) GS232Parse(1);
    } else if (mode == manual){
			if (UARTStatus()) GS232Parse(0);
			step = encoderAzGet();
      if (step){
        if (((step > 0) && dirAllowed.right) || ((step < 0) && dirAllowed.left)){
          targetAzimuth += step;
          /* TODO не позволять ускорению вылезти за overlap zone */
          if (targetAzimuth >= 360) targetAzimuth -= 360;
          if (targetAzimuth < 0) targetAzimuth += 360;
        }
      }
    }


		/*=========================================*/
		/* Перевод координат "энкодера" в градусы  */
		antAzimuth = ((360 << 4) / cfgAz.count * antAzimuthPos) >> 4; /* сдвиг для "увеличения точности" */
		antElevation = ((360 << 4) / cfgEl.count * antElevationPos) >> 4; /* сдвиг для "увеличения точности" */

		/*=========================================*/
		/* Поворот антенны                         */
		if (targetAzimuth != antAzimuth){
			/* Перевод градусов в координаты "энкодера" */
			targetAzimuthPos = ((((360 << 4) / cfgAz.count) * targetAzimuth) >> 4); /* сдвиг для "увеличения точности" */
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

		if (ggg){
			if (azimuthTick > 0)
				UARTSendChar('>');
			else if (azimuthTick < 0)
				UARTSendChar('<');
			else
				UARTSendChar('|');
			UARTSendChar(antAzimuthPos / 100 + '0');
			UARTSendChar(antAzimuthPos % 100 / 10 + '0');
			UARTSendChar(antAzimuthPos % 10 + '0');
			UARTSendChar('\r');
			UARTSendChar('\n');
		}

		/*=========================================*/
		/* Интерфейс пользователя                  */
    printUI();
  }

  return 0;
}


#ifdef __SDCC
/* SDCC умеет в прерывания только внутри файла, содержащего main()... Костыль...   */
/* SDCC only generates an interrupt vector table for the file that contains main() */
#include <8052.h>
#include "encoder_hw.h"
void Serial_ISR() __interrupt SI0_VECTOR
{
	UARTInt();
}

void Timer0_ISR() __interrupt TF0_VECTOR
{
	timer0Int();
}

void INT0_ISR() __interrupt IE0_VECTOR
{
  azimuthImpulse();
}

void INT1_ISR() __interrupt IE1_VECTOR
{
  elevationImpulse();
}
#endif

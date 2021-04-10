#include <stdlib.h>

#include "clock.h"
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
#include "eeprom.h"
#include "i2c.h"

/* Конфигурация */
extern struct config cfg;

#define DEBUG_IMPULSE

/* Текущее положение антенны в градусах */
int16_t antAzimuth = 0;
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
int16_t virtualZero;
int16_t tmpAntAzimuth;
int16_t tmpTargetAzimuth;


/*=========================================*/
/* Расчет зоны overlap                     */
static void azConvert(void)
{
	/* Поворот координат на виртуальный 0    */
	tmpAntAzimuth = antAzimuth - virtualZero;
	if (tmpAntAzimuth < 0) tmpAntAzimuth += 360;
	if (tmpAntAzimuth >= 360) tmpAntAzimuth -= 360;
	tmpTargetAzimuth = targetAzimuth - virtualZero;
	if (tmpTargetAzimuth < 0) tmpTargetAzimuth += 360;
	if (tmpTargetAzimuth >= 360) tmpTargetAzimuth -= 360;

	/* Переход от 0 - 360 в -180 - 180       */
	tmpAntAzimuth = (tmpAntAzimuth > 180)?(tmpAntAzimuth - 360):tmpAntAzimuth;
	tmpTargetAzimuth = (tmpTargetAzimuth > 180)?(tmpTargetAzimuth - 360):tmpTargetAzimuth;

	/* С какой стороны антенны провод */
	if (tmpAntAzimuth == 0){
		/* В виртуальном нуле, состояние провода */
		/* не определено                         */
		dirAllowed.wire_right = 0;
		dirAllowed.wire_left = 0;
	} else {
		if (dirAllowed.wire_right == 0 && dirAllowed.wire_left == 0){
			/* Начали движение от нуля, все просто */
			/* Иные случаи игнорируем */
			if (tmpAntAzimuth > 0){
				dirAllowed.wire_left = 1;
			} else{
				dirAllowed.wire_right = 1;
			}
		}
		if (dirAllowed.wire_right && dirAllowed.wire_left){
			/* Такой ситуации не может быть. */
			dirAllowed.wire_left = 0;
			dirAllowed.wire_right = 0;
		}
	}

	/* Расширение шкалы -180 - 180 на размер overlap зоны */
	if (dirAllowed.wire_left){
		if (tmpAntAzimuth < 0) tmpAntAzimuth += 360;
		if (tmpTargetAzimuth < 0 && tmpTargetAzimuth <= (-180 + cfg.Az.overlap_size)) tmpTargetAzimuth += 360;
	}
	if (dirAllowed.wire_right){
		if (tmpAntAzimuth > 0) tmpAntAzimuth -= 360;
		if (tmpTargetAzimuth > 0 && tmpTargetAzimuth >= (180 - cfg.Az.overlap_size)) tmpTargetAzimuth -= 360;
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
		if (tmpTargetAzimuth == (-180 - cfg.Az.overlap_size)){
			/* Дошли до края overlap зоны */
			dirAllowed.left = 0;
			dirAllowed.left_overlap = 1;
		} else if ((tmpTargetAzimuth <= 0) && tmpTargetAzimuth > -180){
			/* Ещё не вошли в overlap зону */
			dirAllowed.left = 1;
			dirAllowed.left_overlap = 0;
		} else if (tmpTargetAzimuth <= 0){
			/* Внутри overlap зоны */
			dirAllowed.left = 1;
			dirAllowed.left_overlap = 1;
		} else {
			/* Переход через 0 */
		}
	} else if (dirAllowed.wire_left){
		/* Провод расположен слева от антенны */
		/* В overlap зону можно заходить по часовой стрелке */
		dirAllowed.left_overlap = 0;
		dirAllowed.left = 1;
		if (tmpTargetAzimuth == (180 + cfg.Az.overlap_size)){
			/* Дошли до края overlap зоны */
			dirAllowed.right = 0;
			dirAllowed.right_overlap = 1;
		} else if ((tmpTargetAzimuth >= 0) && tmpTargetAzimuth < 180){
			/* Ещё не вошли в overlap зону */
			dirAllowed.right = 1;
			dirAllowed.right_overlap = 0;
		} else if (tmpTargetAzimuth >= 0){
			/* Внутри overlap зоны */
			dirAllowed.right = 1;
			dirAllowed.right_overlap = 1;
		} else {
			/* Переход через 0 */
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

/* TODO костыль из-за недостатка памяти. */
extern int16_t LCDAntAzimuth;

extern int8_t azimuthTick;
int main (void)
{
	int8_t step;

	/* Задержка для включения LCD экрана. А нужна ли? */
  delay_hw_ms(50);

  /* Инициализация железа для работы с EEPROM */
	I2C_Init();

	/* Чтение конфигурации из EEPROM */
	readConfig();

	/* Инициализация остального железа */
	clockInit();
  LCDInit();
  motorsInit();
  encoderInit();
	UARTInit();
  rotateInit();

  /* Остановить все движение */
  motorAzStop();
  motorElStop();

	/* Чтение положения из EEPROM */
  readAnt();
	antAzimuth = azP2D(antAzimuthPos);
	if (cfg.Flags.el_enable){
		antElevation = elP2D(antElevationPos);
	}
	targetAzimuth = antAzimuth;

	/*=========================================*/
	/* Вход в режим настройки                  */
	if (encoderAzBtnGet(1) && encoderElBtnGet(1)){
		configure();
	}

  /* Расчет виртуального нуля             */
	/* 180 - overlap pos                    */
	if (cfg.Az.overlap_position >= 180){
		virtualZero = cfg.Az.overlap_position - 180;
	} else {
		virtualZero = cfg.Az.overlap_position + 180;
	}
	virtualZero = (virtualZero > 180)?(virtualZero - 360):virtualZero;

  startupMessage();
	delay_hw_ms(800);

	/*=========================================*/
	/* Вход в режим калибровки азимута         */
	if (encoderAzBtnGet(1) && !encoderElBtnGet(1)){
		calibrateAz();
	}

	/*=========================================*/
	/* Вход в режим калибровки элевации        */
	if (cfg.Flags.el_enable && !encoderAzBtnGet(1) && encoderElBtnGet(1)){
		calibrateEl();
	}

	azConvert();
	calcDir();

	mode = port;

	initUI();
	printUI();

	struct timer actionTimer;

  while(1){
		/*=========================================*/
		/* Смена режимов работы                    */
    if (encoderElBtnGet(0)){
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
			/* Переменная step не задействована в этом режиме */
			/* Послужит флагом изменения целевого значения    */
			if (UARTStatus()) step = GS232Parse(1);
    } else if (mode == manual){
			if (UARTStatus()) GS232Parse(0);
			step = encoderAzGet();
      if (step){
				if (encoderAzBtnGet(1)){
					step *= 10;
				}
        if (((step > 0) && dirAllowed.right) || ((step < 0) && dirAllowed.left)){
          /* Движение разрешено, если не нарушает overlap зону */
          /* Или нажата кнопка энкодера, как подтверждение действия */
          /* Но при этом провод все равно не будет перекручен. */
					timerReload(&actionTimer, 2000);
					targetAzimuth += step;

          if ((tmpTargetAzimuth + step) > (180 + cfg.Az.overlap_size)){
            targetAzimuth = cfg.Az.overlap_position + cfg.Az.overlap_size;
          }

          if ((tmpTargetAzimuth + step) < (-180 - cfg.Az.overlap_size)){
            targetAzimuth = cfg.Az.overlap_position - cfg.Az.overlap_size;
          }

          if (targetAzimuth >= 360) targetAzimuth -= 360;
          if (targetAzimuth < 0) targetAzimuth += 360;
        } else {
					step = 0;
				}
      }
    }

		/*=========================================*/
		/* Обработка зоны overlap                  */
		azConvert();

		if (1 || step){
			/* Целевое значение менялось */
			calcDir();
		}


		/*=========================================*/
		/* Перевод координат "энкодера" в градусы  */
		antAzimuth = azP2D(antAzimuthPos);
		if (cfg.Flags.el_enable){
			antElevation = elP2D(antElevationPos);
		}


		/*=========================================*/
		/* Сохранение текущего положения в eeprom  */
		if (antAzimuth != LCDAntAzimuth){
			writeAnt();
		}

		/*=========================================*/
		/* Поворот антенны                         */
		if (targetAzimuth != antAzimuth){
			/* Задержка начала движения при изменении */
			/* Целевого значения вручную */
			if (mode == port || timerCheck(&actionTimer)){
				/* Перевод градусов в координаты "энкодера" */
				targetAzimuthPos = ((((360 << 4) / cfg.Az.count) * targetAzimuth) >> 4); /* сдвиг для "увеличения точности" */

				if (tmpAntAzimuth < tmpTargetAzimuth){
					motorAzRight();
				} else {
					motorAzLeft();
				}
			}
		} else {
			motorAzStop();
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

#ifdef DEBUG_IMPULSE
uint8_t debug_impulse_count = 0;
#endif

void Timer2_ISR() __interrupt 5 //TF2_VECTOR
{
	TF2 = 0;

	encoderAzRead();
	encoderElRead();

#ifdef DEBUG_IMPULSE
	if (debug_impulse_count == 0){
		azimuthImpulse();
		debug_impulse_count = RCAP2H >> 1;
	}
	debug_impulse_count--;
#endif
}

#endif

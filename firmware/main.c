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

/* Значения с прошлой итерации цикла */
int16_t oldAntAzimuth = 0;
int16_t oldAntElevation = 0;


/* Целевое значение положения антенны в градусах */
int16_t targetAzimuth;
int16_t targetElevation;

/* Текущее положение антенны в "тиках" "энкодера" */
extern int16_t antAzimuthPos;
extern int16_t antElevationPos;

/* Целевое значение положения антенны в "тиках" "энкодера" */
extern int16_t targetAzimuthPos;
extern int16_t targetElevationPos;

/* Режим работы */
enum workMode mode;

/* Допустимые направления движения */
struct dir dirAllowed;

/* Переменные для обработки зоны overlap */
int16_t virtualZero;
int16_t tmpAntAzimuth;
int16_t tmpTargetAzimuth;

/* Переменная с событиями */
union triggers trigger;

#ifdef __SDCC
/* SDCC требует прототипы прерываний только внутри файла, содержащего main()...  */
#include <8052.h>
void Serial_ISR(void) __interrupt (SI0_VECTOR);
void Timer0_ISR(void) __interrupt (TF0_VECTOR);
void INT0_ISR(void) __interrupt (IE0_VECTOR);
void INT1_ISR(void) __interrupt (IE1_VECTOR);
void Timer2_ISR(void) __interrupt (5); //TF2_VECTOR
#endif


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

extern int8_t azimuthTick;
int main (void)
{
	int8_t step;

  /* Остановить все движение */
  motorsInit();
  motorAzStop();
  motorElStop();

	/* Задержка для включения LCD экрана. А нужна ли? */
  delay_hw_ms(50);

  /* Инициализация железа */
	I2C_Init();
	clockInit();
  LCDInit();
  encoderInit();
	UARTInit();
  rotateInit();

	/* Чтение конфигурации из EEPROM */
	readConfig();




	/*=========================================*/
	/* Вход в режим настройки                  */
	/* При удержании обоих энкодеров или несовпадении контрольной суммы  */
	if (readConfig() || (encoderAzBtnGet(1) && encoderElBtnGet(1))){
		configure();
	}

	/* Чтение положения из EEPROM */
  readAnt();
	antAzimuth = azP2D(antAzimuthPos);
	if (cfg.Flags.el_enable){
		antElevation = elP2D(antElevationPos);
	}



	/*=========================================*/
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
	if (encoderAzBtnGet(BTN_CONT) && !encoderElBtnGet(BTN_CONT)){
		calibrateAz();
		antAzimuth = azP2D(antAzimuthPos);
	}

	/*=========================================*/
	/* Вход в режим калибровки элевации        */
	if (cfg.Flags.el_enable && !encoderAzBtnGet(BTN_CONT) && encoderElBtnGet(BTN_CONT)){
		calibrateEl();
		antElevation = elP2D(antElevationPos);
	}


	/*=========================================*/
	/* Уставка должна совпадать с текущим      */
	/* положением при старте                   */
	targetAzimuth = antAzimuth;
	targetElevation = antElevation;


	//
	mode = WORK_MANUAL;

	// Расчет зоны overlap
	azConvert();

	// Расчет допустимых направлений
	calcDir();

	// Тригерим все события для корректной отрисовки интерфейса
	trigger.all = 0xff;

	initUI();
	printUI();

	struct timer actionTimer;

  while(1){
		/* Очистка переменной с событиями */
		trigger.all = 0;

		/*=========================================*/
		/* Смена режимов работы                    */
    if (encoderElBtnGet(0)){
			trigger.t.modeChange = 1;
      if (mode == WORK_PORT){
				mode = WORK_MANUAL;
				// сброс накопленного за время работы счетчика энкодера
				encoderAzGet();
				// Сброс уставки, чтобы не начинать движение при смене режима
        targetAzimuth = antAzimuth;
			} else if (mode == WORK_MANUAL){
				mode = WORK_PORT;
			}
    }

    /*=========================================*/
		/* Логика выбранного режима                */
		if (mode == WORK_PORT){
			/* Переменная step не задействована в этом режиме */
			/* Послужит флагом изменения целевого значения    */
			if (UARTStatus()) step = GS232Parse(1);
    } else if (mode == WORK_MANUAL){
			// Обработка команды чтения положения с UART
			if (UARTStatus()) GS232Parse(0);

			//
			step = encoderAzGet();
			uint8_t btn = encoderAzBtnGet(BTN_CONT);
      if (step){
				// Увеличивать шаг только вне зоны overlap
				if (btn && !(dirAllowed.right_overlap || dirAllowed.left_overlap)){
					step *= 10;
				}

        /* Движение разрешено, если не нарушает overlap зону */
        /* Или нажата кнопка энкодера, как подтверждение действия */
        /* Но при этом провод все равно не будет перекручен. */
        if (((step > 0) && dirAllowed.right) || ((step < 0) && dirAllowed.left) || btn){
					timerReload(&actionTimer, 2000);
					targetAzimuth += step;
          if (targetAzimuth >= 360) targetAzimuth -= 360;
          if (targetAzimuth < 0) targetAzimuth += 360;
					trigger.t.azTargetChange = 1;
        } else {
					step = 0;
				}
      }
    }

		/*=========================================*/
		/* Обработка зоны overlap                  */
		azConvert();

		if (trigger.t.azTargetChange){
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
		if (antAzimuth != oldAntAzimuth){
			oldAntAzimuth = antAzimuth;
			trigger.t.azChange = 1;
			writeAnt();
		}

		/*=========================================*/
		/* Поворот антенны                         */
		if (targetAzimuth != antAzimuth){
			/* Задержка начала движения при изменении */
			/* Целевого значения вручную */
			if (mode == WORK_PORT || timerCheck(&actionTimer)){
				if (tmpAntAzimuth < tmpTargetAzimuth){
					motorAzRight();
				} else {
					motorAzLeft();
				}
				// Взводим триггер, так как включали двигатель
				trigger.t.driveChange = 1;
			}
		} else {
			// Взводим триггер, если двигатель был включен
			if (motorAzStatus())
				trigger.t.driveChange = 1;

			motorAzStop();
		}

		/*=========================================*/
		/* Индикация работы двигателей */
		P2_0 = !motorAzStatus();

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
void Serial_ISR(void) __interrupt (SI0_VECTOR)
{
	UARTInt();
}

void Timer0_ISR(void) __interrupt (TF0_VECTOR)
{
	timer0Int();
}

void INT0_ISR(void) __interrupt (IE0_VECTOR)
{
  azimuthImpulse();
}

void INT1_ISR(void) __interrupt (IE1_VECTOR)
{
  elevationImpulse();
}

#ifdef DEBUG_IMPULSE
uint8_t debug_impulse_count = 0;
#endif

void Timer2_ISR(void) __interrupt (5) //TF2_VECTOR
{
	TF2 = 0;

	encoderAzRead();
	encoderElRead();

	#ifdef DEBUG_IMPULSE
	if (debug_impulse_count == 0){
		azimuthImpulse();
		debug_impulse_count = 50;//RCAP2H >> 1;
	}
	debug_impulse_count--;
	#endif
}

#endif

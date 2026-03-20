#include <stdio.h>
#include "configure.h"
#include "ui.h"
#include "lcd.h"
#include "rotate.h"
#include "motors.h"

/* Конфигурация устройства */
extern struct config cfg;

/* Текущее положение антенны */
extern int16_t antAzimuth;
extern int16_t antElevation;

/* Целевое значение положения */
extern int16_t targetAzimuth;
extern int16_t targetElevation;

/* Режим работы */
extern enum workMode mode;

/* Допустимые направления */
extern struct dir dirAllowed;

/* События */
extern union triggers trigger;

/* Локальные переменные */
uint8_t antAzDig[3];
uint8_t antElDig[3];

void startupMessage(void)
{
  LCDClear();
	LCDPosition(0, 0); LCDPrintString("RK3MXT Rotator");
	LCDPosition(0, 1); LCDPrintString("Version 0.2");
}

#define debug_mode 0

/* Интерфейс с большими цифрами */
void printBigAnt()
{
  static uint8_t LCDD1 = 0xff;
  static uint8_t LCDD2 = 0xff;

	LCDNormal();

	// Выводим только при изменении
	// Экономим время на тормозном обмене с LCD
  if (LCDD1 != antAzDig[0]){
    LCDD1 = antAzDig[0];
		if (LCDD1 != 0){
			LCDPrintBigDigit(LCDD1, 7);
		} else {
			LCDPrintBigDigit(' ', 7);
		}
  }
  if (LCDD2 != antAzDig[1]){
    LCDD2 = antAzDig[1];
		if (LCDD1 != 0 || LCDD2 != 0){
			LCDPrintBigDigit(LCDD2, 10);
		} else {
			LCDPrintBigDigit(' ', 10);
		}
  }

	// Последняя цифра всегда изменяется
	LCDPrintBigDigit(antAzDig[2], 13);
}

void azOnlyUI(void){
	/* Режим работы */
  if (trigger.t.modeChange){
		LCDNormal();
		LCDPosition(0, 0);

		if (mode == WORK_PORT){
			LCDPrintString("Port  ");
		} else {
			LCDPrintString("Manual");
		}
  } else if (trigger.t.driveChange){
		LCDNormal();
		LCDPosition(0, 0);
		int8_t tmp = motorAzTick();
		if (tmp){
			char l = ' ';
			char r = ' ';
			if (tmp < 0)
				l = '<';
			else
				r = '>';

			LCDPrintChar(l);
			LCDPrintString("Run ");
			LCDPrintChar(r);
		} else {
			LCDPrintString(" Stop ");
		}
	}

	/* Целевое значение азимута */
  if (trigger.t.azTargetChange){
		LCDReverse(); // числа выводятся в обратном порядке
    LCDPosition(5, 1);

		if (dirAllowed.right_overlap || dirAllowed.left_overlap)
			LCDPrintChar('!');
		else
			LCDPrintChar(' ');

    if (dirAllowed.right)
      LCDPrintChar(dirAllowed.right_overlap?')':'>');
    else
      LCDPrintChar(']');

		LCDPrint(targetAzimuth, 3);

		if (dirAllowed.left)
      LCDPrintChar(dirAllowed.left_overlap?'(':'<');
    else
      LCDPrintChar('[');
  }

	/* Текущее положение */
	if (trigger.t.azChange){
		printBigAnt();
	}
}


void printUI(void){
	// Извлечение цифр, используется далее в нескольких местах
	antAzDig[0] = antAzimuth / 100;
	antAzDig[1] = antAzimuth % 100 / 10;
	antAzDig[2] = antAzimuth % 10;

	if (!cfg.Flags.el_enable){
		azOnlyUI();
	} else {
		antElDig[0] = antElevation / 100;
		antElDig[1] = antElevation % 100 / 10;
		antElDig[2] = antElevation % 10;
	}
}

void initUI(void){
	LCDClear();
}

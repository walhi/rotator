#include <stdio.h>
#include "configure.h"
#include "ui.h"
#include "lcd.h"
#include "rotate.h"

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

/* Локальные переменные */
enum workMode oldMode = none;
int16_t LCDAntAzimuth = 0x7fff;
static int16_t LCDAntElevation = 0x7fff;
static int16_t LCDTargetAzimuth = 0x7fff;
static int16_t LCDTargetElevation = 0x7fff;

void startupMessage()
{
  LCDClear();
	LCDPosition(0, 0); LCDPrintString("Alt. firmware");
	LCDPosition(0, 1); LCDPrintString("RK3MXT Rotator");
}

/* Классический интерфейс */
void oldPrintMode(void)
{
	LCDNormal();
	LCDPosition(0, 0);
	if (mode == port){
		LCDPrintString("PORT");
	} else {
		LCDPrintString(" MAN");
	}
	LCDReverse(); // числа выводятся в обратном порядке
}

void oldInit(void)
{
	LCDClear();
	LCDNormal();
	LCDPosition(0, 0);
	if (cfg.Flags.el_enable){
		LCDPosition(4, 0);
		LCDPrintString(" AZ    EL");
		LCDPosition(0, 1);
		LCDPrintString(" ANT");
		LCDPrintString(" AZ    EL");
	} else {
		LCDPosition(4, 0);
		LCDPrintString(" AZIMUTH ");
		LCDPosition(0, 1);
		LCDPrintString(" ANT");
		LCDPrintString(" AZIMUTH ");
	}
	oldPrintMode();
}

void oldPrintTarget(void)
{
	if (cfg.Flags.el_enable){
		LCDPosition(9, 0);
		LCDPrint(targetAzimuth, 3);
		LCDPosition(15, 0);
		LCDPrint(targetElevation, 3);
	} else {
		LCDPosition(15, 0);
		LCDPrint(targetAzimuth, 3);
	}
}

void oldPrintAnt(void)
{
	if (cfg.Flags.el_enable){
		LCDPosition(9, 1);
		LCDPrint(antAzimuth, 3);
		LCDPosition(15, 1);
		LCDPrint(antElevation, 3);
	} else {
		LCDPosition(15, 1);
		LCDPrint(antAzimuth, 3);
	}
}

void oldPrintUI(void)
{
  if (oldMode != mode){
    oldMode = mode;
		oldPrintMode();
  }
	if (LCDTargetAzimuth != targetAzimuth){
		LCDTargetAzimuth = targetAzimuth;
    oldPrintTarget();
  }
	if (LCDAntAzimuth != antAzimuth){
		LCDAntAzimuth = antAzimuth;
		oldPrintAnt();
  }
}

extern int16_t virtualZero;
extern int16_t tmpAntAzimuth;
extern int16_t tmpTargetAzimuth;


/* Интерфейс с большими цифрами */
#ifdef BIG_FONT
void printBigAnt(int16_t value)
{
  static uint8_t LCDD1 = 0xff;
  static uint8_t LCDD2 = 0xff;
  static uint8_t LCDD3 = 0xff;
  uint8_t d1 = value / 100;
  uint8_t d2 = value % 100 / 10;
  uint8_t d3 = value % 10;
  if (LCDD1 != d1){
    LCDD1 = d1;
		if (d1 != 0){
			LCDPrintBigDigit(d1, 7);
		} else {
			LCDPrintBigDigit(' ', 7);
		}
  }
  if (LCDD2 != d2){
    LCDD2 = d2;
		if (d1 != 0 || d2 != 0){
			LCDPrintBigDigit(d2, 10);
		} else {
			LCDPrintBigDigit(' ', 10);
		}
  }
  if (LCDD3 != d3){
    LCDD3 = d3;
    LCDPrintBigDigit(d3, 13);
  }
}
#endif

void newPrintUI(void){
	/* Режим работы */
  if (oldMode != mode){
    oldMode = mode;
		LCDNormal();
		LCDPosition(0, 0);
		if (mode == port){
			LCDPrintString("Port  ");
		} else {
			LCDPrintString("Manual");
		}
  }

	/* Целевое значение азимута */
  if (LCDTargetAzimuth != targetAzimuth){
    LCDTargetAzimuth = targetAzimuth;
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
	if (LCDAntAzimuth != antAzimuth){
		LCDAntAzimuth = antAzimuth;
		LCDNormal();
#ifdef BIG_FONT
		printBigAnt(antAzimuth);
#endif
	}

}

void printUI(){
  if (cfg.Flags.ui_use_old){
    oldPrintUI();
  } else {
    newPrintUI();
  }
}

void initUI(){
  if (cfg.Flags.ui_use_old){
    oldInit();
  } else {
		LCDClear();
	}
}

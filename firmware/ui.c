#include <stdio.h>
#include "configure.h"
#include "ui.h"
#include "lcd.h"

/* Конфигурация устройства */
extern struct configFlags cfgFlags;

/* Текущее положение антенны */
extern int16_t antAzimuth;
extern int16_t antElevation;

/* Целевое значение положения */
extern int16_t targetAzimuth;
extern int16_t targetElevation;

/* Режим работы */
extern enum workMode mode;

/* Текущее направление движения */
extern int8_t azimuthTick;

/* Локальные переменные */
enum workMode oldMode = none;
int16_t LCDAntAzimuth = 0x7fff;
int16_t LCDAntElevation = 0x7fff;
int16_t LCDTargetAzimuth = 0x7fff;
int16_t LCDTargetElevation = 0x7fff;

void startupMessage()
{
  LCDClear();
	LCDPosition(0, 0); LCDPrintString("Alt. firmware");
	LCDPosition(0, 1); LCDPrintString("RK3MXT Rotator");
}


/* Классический интерфейс */

void oldPrintMode(void){
	LCDPosition(0, 0);
	if (mode == port){
		LCDPrintString("PORT");
	} else {
		LCDPrintString(" MAN");
	}
}

void oldInit(void){
	LCDClear();
	LCDPosition(0, 0);
	if (cfgFlags.el_enable){
		LCDPosition(4, 0);
		LCDPrintString(" AZ    EL");
		LCDPosition(4, 1);
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


void oldPrintTarget(void){
	LCDPosition(13, 0);
  /* TODO придумать условие, когда не выводить значение с порта */
  LCDPrintf("%3u", targetAzimuth);
}

void oldPrintAnt(void){
	LCDPosition(13, 1);
	LCDPrintf("%3u", antAzimuth);
}

void oldPrintUI(void){
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


void newPrintUI(void){
}

/* Интерфейс с большими цифрами */

void printBigAnt(int16_t value)
{
  static uint8_t LCDD1 = 0;
  static uint8_t LCDD2 = 0;
  static uint8_t LCDD3 = 0;
  uint8_t d1 = value / 100;
  uint8_t d2 = value % 100 / 10;
  uint8_t d3 = value % 10;
  if (LCDD1 != d1){
    LCDD1 = d1;
    LCDPrintBigDigit(d1, 7);
  }
  if (LCDD2 != d2){
    LCDD2 = d2;
    LCDPrintBigDigit(d2, 10);
  }
  if (LCDD3 != d3){
    LCDD3 = d3;
    LCDPrintBigDigit(d3, 13);
  }
}


void printUI(){
  if (cfgFlags.ui_use_old){
    oldPrintUI();
  } else {
    newPrintUI();
  }
}

void initUI(){
  if (cfgFlags.ui_use_old){
    oldInit();
  }
}

#include "platform.h"
#include "calibration.h"
#include "rotate.h"
#include "delay_hw.h"
#include "configure.h"
#include "encoder.h"
#include "lcd.h"
#include "eeprom.h"

/* Конфигурация */
extern struct config cfg;

extern int16_t antAzimuthPos;
extern int16_t antElevationPos;

extern int16_t antAzimuth;
extern int16_t antElevation;

//CODE_MEM_BEFORE const uint8_t STR1[14] CODE_MEM_AFTER = {'A', 'z', ' ', 'c', 'a', 'l', 'i', 'b', 't', 'a', 'r', 'i', 'o', 'n'};

void calibrateAz(void){
	// Заголовок
  LCDClear();
	LCDPosition(0, 0);
	LCDNormal();

	LCDPrintString("Az calibration");
	//LCDWriteBuf(STR1, 14);

	// Реверс печати на дисплей для удобства
	LCDReverse();

	// Текущее положение
	LCDPosition(15, 1);
	LCDPrint(antAzimuth, 3);
	while(1){
		int8_t step = encoderAzGet();
		if (step){
			antAzimuth += step;
			if (antAzimuth < 0) antAzimuth += 360;
			if (antAzimuth > 360) antAzimuth -= 360;
			LCDPosition(15, 1);
			LCDPrint(antAzimuth, 3);
		}
		if (encoderAzBtnGet(BTN_SINGLE)){
			antAzimuthPos = azD2P(antAzimuth);
      writeAnt();
			break;
		}
	}
}

void calibrateEl(void){
	/*
  LCDClear();
	LCDPosition(0, 0);
	LCDNormal();
	LCDPrintString("El calibration");
	while(1){
		if (encoderElBtnGet(0)){
			break;
		}
	}
	*/
}

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

void calibrateAz(){
  LCDClear();
	LCDPosition(0, 0);
	LCDNormal();
	LCDPrintString("Az calibration");

	LCDReverse();
	while(1){
		int8_t step = encoderAzGet();
		if (step){
			antAzimuth += step;
			if (antAzimuth < 0) antAzimuth += 360;
			if (antAzimuth > 360) antAzimuth -= 360;
			LCDPosition(15, 1);
			LCDPrint(antAzimuth, 3);
		}
		if (encoderAzBtnGet(0)){
			antAzimuthPos = azD2P(antAzimuth);
      writeAnt();
			break;
		}
	}
}

void calibrateEl(){
  LCDClear();
	LCDPosition(0, 0);
	LCDNormal();
	LCDPrintString("El calibration");
	while(1){
		if (encoderElBtnGet(0)){
			break;
		}
	}
}

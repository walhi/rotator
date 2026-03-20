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
	antAzimuth = configureInt("Az calibration", 0, 360, antAzimuth, 1);
	antAzimuthPos = azD2P(antAzimuth);
  writeAnt();
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

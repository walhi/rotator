#include "calibration.h"
#include "delay_hw.h"
#include "configure.h"
#include "encoder.h"
#include "lcd.h"

/* Конфигурация */
extern struct configAz cfgAz;
extern struct configEl cfgEl;
extern struct configFlags cfgFlags;


void calibrateAz(){
  LCDClear();
	LCDPosition(0, 0);
	LCDNormal();
	LCDPrintString("Az calibration");
	while(1){
		if (encoderAzBtnGet(0)){
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

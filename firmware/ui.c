#include <stdio.h>
#include <util/delay.h>
#include "ui.h"
#include "lcd.h"

extern uint16_t antAzimuth;
extern uint16_t targetAzimuth;

extern enum workMode mode;

void startupMessage()
{
  LCDClear();
	LCDPosition(0, 0); LCDPrintString("Alt. firmware");
	LCDPosition(0, 1); LCDPrintString("RK3MXT Rotator");
  _delay_ms(1000);
}


void printHeader(){
  LCDClear();
	LCDPosition(0, 0);
	if (mode == port){
		LCDPrintString("PORT AZIMUTH");
	} else {
		LCDPrintString(" MAN AZIMUTH");
	}
	LCDPosition(0, 1);
	LCDPrintString(" ANT AZIMUTH");
}

void printTarget(){
	LCDPosition(13, 0);
	if (targetAzimuth < 360){ /* init value: 0xffff */
  	LCDPrintf("%3u", targetAzimuth);
	} else {
    LCDPrintChar(' ');
	}
}

void printAnt(){
	LCDPosition(13, 1);
	LCDPrintf("%3u", antAzimuth);
}

void printValues(){
  printTarget();
  printAnt();
}

void printUI(){
	printHeader();
	printValues();
}

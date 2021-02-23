#include "encoder.h"
#include "encoder_hw.h"
#include "delay_hw.h"

static const uint16_t rot_enc_table = 0b0110100110010110;

void encoderInit()
{
  encoderHwInit();
}

/* A vald CW or CCW move returns 1, invalid returns 0. */
/* https://www.best-microcontroller-projects.com/rotary-encoder.html */
int8_t encoderAzGet()
{
	static uint8_t  prevNextCode = 0;
	static uint16_t store = 0;
  static int8_t oldDir = 0;
  static int8_t tick = 1;
  int8_t dir = 0;

  prevNextCode <<= 2;
  if (encoderHwAzAGet()) prevNextCode |= 0x02;
  if (encoderHwAzBGet()) prevNextCode |= 0x01;
  prevNextCode &= 0x0f;

	if ((rot_enc_table >> prevNextCode) & 0x01) {
		store <<= 4;
		store |= prevNextCode;
		if ((store & 0xff) == 0x2b) dir = -1;
		if ((store & 0xff) == 0x17) dir = 1;
	}


	return dir;
}


int8_t encoderElGet()
{
	static uint8_t  prevNextCode = 0;
	static uint16_t store = 0;

  prevNextCode <<= 2;
  if (encoderHwElAGet()) prevNextCode |= 0x02;
  if (encoderHwElBGet()) prevNextCode |= 0x01;
  prevNextCode &= 0x0f;

	if ((rot_enc_table >> prevNextCode) & 0x01) {
		store <<= 4;
		store |= prevNextCode;
		if ((store&0xff)==0x2b) return -1;
		if ((store&0xff)==0x17) return 1;
	}

  return 0;
}

int8_t encoderAzBtnGet(uint8_t cont)
{
  static uint8_t encoderAzBtn = 1; /* extern pull up */
	uint8_t state = encoderHwAzBtnGet();
	if (state != encoderAzBtn){
		encoderAzBtn = state;
		delay_hw_ms(10);
		if (state == 0) return 1;
	}
	if (cont){
		if (state == 0) return 1;
	}
	return 0;
}

int8_t encoderElBtnGet(uint8_t cont)
{
  static uint8_t encoderElBtn = 1; /* extern pull up */
	uint8_t state = encoderHwElBtnGet();
	if (state != encoderElBtn){
		encoderElBtn = state;
		delay_hw_ms(10);
		if (state == 0) return 1;
	}
	if (cont){
		if (state == 0) return 1;
	}
	return 0;
}

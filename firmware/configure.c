#include <stdio.h>
#include "platform.h"
#include "delay_hw.h"
#include "encoder.h"
#include "configure.h"
#include "lcd.h"
#include "eeprom.h"

struct config cfg;

#define CONFIG_ENABLE
#ifdef CONFIG_ENABLE

#ifdef __SDCC
__code const char enable_msg[]  = " Enable";
__code const char disable_msg[] = "Disable";
#else
#define enable_msg  " Enable"
#define disable_msg "Disable"
#endif
#define PRINT_FLAG_VALUE {\
	LCDPosition(9, 1);\
	if (value){\
		LCDPrintString(enable_msg);\
	} else {\
		LCDPrintString(disable_msg);\
	}\
}

uint8_t configureFlag(char* name, uint8_t value)
{
	uint8_t oldValue = value;
	int8_t encoder;
  LCDClear();
  LCDPosition(0, 0);
  LCDPrintString(name);
	PRINT_FLAG_VALUE;

	while (1){
		encoder = encoderAzGet();

		if (encoder){
			if (encoder > 0){
				value = 1;
			} else {
				value = 0;
			}

			PRINT_FLAG_VALUE;
			LCDPosition(0, 1);
			if (oldValue != value){
				LCDPrintChar('*');
			} else {
				LCDPrintChar(' ');
			}
		}
    if (encoderElBtnGet(0)) break;
	}
	return value;
}

#define PRINT_VALUE {LCDPosition(15, 1); LCDPrint(value, 5);}

int16_t configureInt(char* name, int16_t min, int16_t max, int16_t currentValue)
{
  int8_t encoder;
	int16_t value = currentValue;

  LCDClear();
  LCDPosition(0, 0);
  LCDPrintString(name);
	LCDReverse(); /* Числа выводятся в обратном порядке */
	PRINT_VALUE;

	while(1){
		encoder = encoderAzGet();
		if (encoder){
			value += encoder;
			if (value < min) value = min;
			if (value > max) value = max;

			PRINT_VALUE;
			LCDPosition(0, 1);
			if (value != currentValue){
				LCDPrintChar('*');
			} else {
				LCDPrintChar(' ');
			}
		}

		if (encoderElBtnGet(0))
			break;
	}
	return value;
}

void configure(void)
{
  LCDClear();
  LCDPosition(0, 0);
	LCDPrintString("Configuraton");
  LCDPosition(0, 1);
	LCDPrintString("editor");
	delay_hw_ms(300);


	cfg.Az.count = configureInt("AZ 360 imp count", 0, 32000, cfg.Az.count);
	cfg.Az.overlap_position = configureInt("AZ overlap pos:", 0, 360, cfg.Az.overlap_position);
	cfg.Az.overlap_size = configureInt("AZ overlap size:", 0, 360, cfg.Az.overlap_size);
	cfg.Az.parking = configureInt("AZ Parking:", 0, 360, cfg.Az.parking);

	cfg.Flags.ui_use_old = configureFlag("Old interface:", cfg.Flags.ui_use_old);
  cfg.Flags.el_enable = configureFlag("Elevation:", cfg.Flags.el_enable);
	if (cfg.Flags.el_enable){
		cfg.El.count = configureInt("EL 180 imp count", 0, 32000, cfg.El.count);
		cfg.El.min = (uint8_t)configureInt("EL start:", 0, 180, cfg.El.min);
		cfg.El.max = (uint8_t)configureInt("EL end:", 0, 180, cfg.El.max);
		cfg.El.parking = (uint8_t)configureInt("EL Parking:", 0, 360, cfg.El.parking);
	}
	writeConfig();
}
#endif

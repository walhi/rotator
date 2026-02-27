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

#ifdef __SDCC && 0
__code const char enable_msg[]  = " Enable";
__code const char disable_msg[] = "Disable";
#else
#define enable_msg  " Enable"
#define disable_msg "Disable"
#endif

void PrintValue(int16_t value, uint8_t digit){
	LCDPosition(15, 1);
	LCDReverse();
	LCDPrint(value, digit);
}

void PrintChangeFlag(uint8_t value){
	LCDPosition(0, 1);
	LCDNormal();
	if (value){
		LCDPrintChar('*');
	} else {
		LCDPrintChar(' ');
	}
}

void PrintFlagValue(uint8_t value){
	LCDPosition(9, 1);
	if (value){
		LCDPrintString(enable_msg);
	} else {
		LCDPrintString(disable_msg);
	}

}

uint8_t configureFlag(char* name, uint8_t value)
{
	uint8_t currentValue = value;
	int8_t encoder;
  LCDClear();
  LCDPosition(0, 0);
  LCDPrintString(name);

	PrintFlagValue(value);
	while (1){
		encoder = encoderAzGet();

		if (encoder){
			if (encoder > 0){
				value = 1;
			} else {
				value = 0;
			}

			PrintFlagValue(value);
			PrintChangeFlag(currentValue != value);
		}
    if (encoderElBtnGet(0))
			break;
	}
	return value;
}

int16_t configureInt(char* name, int16_t min, int16_t max, int16_t currentValue)
{
  int8_t encoder;
	int16_t value = currentValue;

  LCDClear();
  LCDPosition(0, 0);
  LCDPrintString(name);

	PrintValue(value, 5);
	while(1){
		encoder = encoderAzGet();
		if (encoder){
			value += encoder;
			if (value < min) value = min;
			if (value > max) value = max;


			PrintValue(value, 5);
			PrintChangeFlag(currentValue != value);
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
	LCDPrintString("Conf. edit");
	delay_hw_ms(300);


	cfg.Az.count = configureInt("AZ 360 imp count", 0, 32000, cfg.Az.count);
	cfg.Az.overlap_position = configureInt("AZ overlap pos:", 0, 360, cfg.Az.overlap_position);
	cfg.Az.overlap_size = configureInt("AZ overlap size:", 0, 360, cfg.Az.overlap_size);
	//cfg.Az.parking = configureInt("AZ Parking:", 0, 360, cfg.Az.parking);

	cfg.Flags.ui_use_old = configureFlag("Old interface:", cfg.Flags.ui_use_old);
  cfg.Flags.el_enable = configureFlag("Elevation:", cfg.Flags.el_enable);
	if (cfg.Flags.el_enable){
		cfg.El.count = configureInt("EL 180 imp count", 0, 32000, cfg.El.count);
		cfg.El.min = (uint8_t)configureInt("EL start:", 0, 180, cfg.El.min);
		cfg.El.max = (uint8_t)configureInt("EL end:", 0, 180, cfg.El.max);
		//cfg.El.parking = (uint8_t)configureInt("EL Parking:", 0, 360, cfg.El.parking);
	}
	writeConfig();
}
#endif

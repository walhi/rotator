#include <stdio.h>
#include "platform.h"
#include "delay_hw.h"
#include "encoder.h"
#include "configure.h"
#include "lcd.h"

struct configAz cfgAz;
struct configEl cfgEl;
struct configFlags cfgFlags;

//#define CONFIG_ENABLE
#ifdef CONFIG_ENABLE

struct menuItem{
	char name[17]; /* 16 + end of string (0x00) */
	int16_t min;
	int16_t max;
	union {
		int16_t* value16;
		uint8_t* value8;
	};
};

#ifdef __SDCC
#define currentItem list[i]
#else
struct menuItem currentItem __attribute__ ((section (".noinit")));
#endif

#define AZ_COUNT 4
CODE_MEM_BEFORE const struct menuItem azList[AZ_COUNT] CODE_MEM_AFTER =
	{
	 {"AZ 360 imp count", 0, 32000, &cfgAz.count},
	 {"AZ overlap pos:", 0, 360, &cfgAz.overlap_position},
	 {"AZ overlap size:", 0, 360, &cfgAz.overlap_size},
	 {"AZ Parking:", 0, 360, &cfgAz.parking},
	};


#define EL_COUNT 4
CODE_MEM_BEFORE const struct menuItem elList[EL_COUNT] CODE_MEM_AFTER =
	{
	 {"EL 180 imp count", 0, 32000, &cfgEl.count},
	 {"EL start:", 0, 180, .value8=&cfgEl.min},
	 {"EL end:", 0, 180, .value8=&cfgEl.max},
	 {"EL Parking:", 0, 360, .value8=&cfgEl.parking},
	};


static void printHeader(char* name)
{
  LCDClear();
  LCDPosition(0, 0);
  LCDPrintString(name);
}

static void printValue(struct menuItem* item, int16_t value, uint8_t mode8)
{
  LCDPosition(11, 1);
  LCDPrintf("%5d", value);

  LCDPosition(0, 1);
	if (mode8){
		if (*item->value8 != value){
			LCDPrintChar('*');
		} else {
			LCDPrintChar(' ');
		}
	} else {
		if (*item->value16 != value){
			LCDPrintChar('*');
		} else {
			LCDPrintChar(' ');
		}
	}
}

#ifdef __SDCC
__code const char enable_msg[] = "Enable";
__code const char disable_msg[] = "Disable";
#else
#define enable_msg  " Enable"
#define disable_msg "Disable"
#endif

uint8_t configureFlag(char* name, uint8_t value)
{
	uint8_t oldValue = value;
	int8_t encoder;
	uint8_t btn;
  LCDClear();
  LCDPosition(0, 0);
  LCDPrintString(name);
	LCDPosition(9, 1);
	if (value){
		LCDPrintString(enable_msg);
	} else {
		LCDPrintString(disable_msg);
	}

	while (1){
		btn = encoderAzBtnGet();
		encoder = encoderAzGet();
    if (btn) break;

		if (encoder){
			if (encoder > 0){
				value = 1;
			} else {
				value = 0;
			}
			LCDPosition(9, 1);
			if (value){
				LCDPrintString(enable_msg);
			} else {
				LCDPrintString(disable_msg);
			}

			LCDPosition(0, 1);
			if (oldValue != value){
				LCDPrintChar('*');
			} else {
				LCDPrintChar(' ');
			}
		}
	}
	return value;
}

void configureInt(const struct menuItem* list, int8_t count)
{
	int8_t i = 0;
  int8_t encoder;
  uint8_t btn;
	int16_t value;

	btn = 1; /* Значение для принудительной */
	i = -1;  /* подгрузки пункта меню       */
#ifdef __GNUC__
	currentItem.value16 = NULL;
#endif

  while(1){
    if (btn){
			i++;
			if (i >= count){
				/* Save to EEPROM */
				i = 0;
				break;
			}

			if (currentItem.value16 != NULL)
				if (currentItem.max > 255){
					*currentItem.value16 = value;
				} else {
					*currentItem.value8 = (uint8_t)value;
				}

			memcpy_P(&currentItem, &list[i], sizeof(struct menuItem));

			printHeader(currentItem.name);
			if (currentItem.max > 255){
				value = *currentItem.value16;
				printValue(&currentItem, value, 0);
			} else {
				value = *currentItem.value8;
				printValue(&currentItem, value, 1);
			}
    }

		encoder = encoderAzGet();

    if (encoder){
      value += encoder;
      if (value < currentItem.min) value = currentItem.min;
      if (value > currentItem.max) value = currentItem.max;
			if (currentItem.max > 255){
				printValue(&currentItem, value, 0);
			} else {
				printValue(&currentItem, value, 1);
			}
    }

		btn = encoderAzBtnGet();
  }
}


#ifdef __SDCC
__code const char conf1_msg[] = "Configuraton";
__code const char conf2_msg[] = "Editor";
__code const char com_echo_msg[] = "COM port echo:";
__code const char az_enc_int_msg[] = "AZ encoder int:";
__code const char el_enable_msg[] = "Elevation:";
__code const char el_enc_int_msg[] = "EL encoder int:";
#else
#define conf1_msg "Configuraton"
#define conf2_msg "Editor"
#define com_echo_msg "COM port echo:"
#define az_enc_int_msg "AZ encoder int:"
#define el_enable_msg "Elevation:"
#define el_enc_int_msg "EL encoder int:"
#endif

void configure(void)
{
  LCDClear();
  LCDPosition(0, 0);
	LCDPrintString(conf1_msg);
  LCDPosition(0, 1);
	LCDPrintString(conf2_msg);
	delay_hw_ms(2000);

	configureInt(azList, AZ_COUNT);
	cfgFlags.com_echo = configureFlag(com_echo_msg, cfgFlags.com_echo);
  cfgFlags.az_enc_int = configureFlag(az_enc_int_msg, cfgFlags.az_enc_int);
  cfgFlags.el_enable = configureFlag(el_enable_msg, cfgFlags.el_enable);
	if (cfgFlags.el_enable){
		configureInt(elList, EL_COUNT);
		cfgFlags.el_enc_int = configureFlag(el_enc_int_msg, cfgFlags.el_enc_int);
	}
}
#endif

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include "encoder.h"
#include "configure.h"
#include "lcd.h"

struct config cfg;

struct menuItem{
	char name[17]; /* 16 + end of string (0x00) */
	int16_t min;
	int16_t max;
	int16_t* value;
};

#define MENU_COUNT 12
const struct menuItem list[MENU_COUNT] PROGMEM = {
                                                  {"AZ 360 imp count:", 0, 32000, &cfg.az_count},
                                                  {"AZ overlap start:", 0, 360, &cfg.az_overlap_start},
                                                  {"AZ overlap end:", 0, 360, &cfg.az_overlap_start},
                                                  {"Elevation:", 0, 1, &cfg.el_enable},
                                                  {"EL 180 imp count:", 0, 32000, &cfg.el_count},
                                                  {"EL start:", 0, 180, &cfg.el_min},
                                                  {"EL end:", 0, 180, &cfg.el_max},
                                                  {"AZ motor invert:", 0, 1, &cfg.az_motor_invert},
                                                  {"AZ encoder inv.:", 0, 1, &cfg.az_enc_invert},
                                                  {"EL motor invert:", 0, 1, &cfg.el_motor_invert},
                                                  {"EL encoder inv.:", 0, 1, &cfg.el_enc_invert},
                                                  {"COM Port echo:", 0, 1, &cfg.com_echo},
};
struct menuItem currentItem;

static void printHeader(struct menuItem* item)
{
  LCDClear();
  LCDPosition(0, 0);
  LCDPrintString(item->name);
}

static void printValue(struct menuItem* item, int16_t value)
{
  if (item->max == 1){
    LCDPosition(9, 1);
    if (value){
      LCDPrintString(" Enable");
    } else {
      LCDPrintString("Disable");
    }
  } else {
    LCDPosition(11, 1);
    LCDPrintf("%5d", value);
  }

  LCDPosition(0, 1);
  if (*item->value != value){
    LCDPrintChar('*');
  } else {
    LCDPrintChar(' ');
  }
}

void configure(void)
{
	uint8_t i;
  int8_t encoder;
  uint8_t btn;
	uint8_t old_btn;
	int16_t value;

  old_btn = btn = PINB & (1 << PB0);

  LCDClear();
	LCDPosition(0, 0); LCDPrintString("Configuraton");
	LCDPosition(0, 1); LCDPrintString("Editor");
	_delay_ms(300);

  memcpy_P(&currentItem, &list[i], sizeof(struct menuItem));
  value = *currentItem.value;
  printHeader(&currentItem);
  printValue(&currentItem, value);

  while(1){
    btn = PINB & (1 << PB0);
    if (old_btn != btn){
      old_btn = btn;
      _delay_ms(50);
			if (btn == 0){
        i++;
        if (i >= MENU_COUNT){
          /* Save to EEPROM */
          i = 0;
          //break;
        }

        *currentItem.value = value;

        memcpy_P(&currentItem, &list[i], sizeof(struct menuItem));
        value = *currentItem.value;
        printHeader(&currentItem);
        printValue(&currentItem, value);
      };
    }
		encoder = encoderAzGet();

    if (encoder){
      value += encoder;
      if (value < currentItem.min) value = currentItem.min;
      if (value > currentItem.max) value = currentItem.max;
      printValue(&currentItem, value);
    }
  }
}

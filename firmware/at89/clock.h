#ifndef CLOCK_H
#define CLOCK_H

#include <stdint.h>

struct timer {
	uint32_t alarmTime;
};

void clockInit(void);

void timerReload(struct timer* t, uint16_t interval);

uint8_t timerCheck(struct timer* t);

void timer0Int(void);


#endif

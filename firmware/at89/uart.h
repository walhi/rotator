#ifndef UART_H
#define UART_H

#include <stdint.h>

void UARTInit(void);

void UARTSend(void);

uint8_t UARTGetLen(void);

uint8_t UARTStatus(void);

void UARTInt(void);

#endif

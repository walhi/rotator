#ifndef UART_H
#define UART_H

#include <stdint.h>

void UARTInit(void);

void UARTSendChar(char data);

void UARTSendBuf(char* data, uint8_t len);

char* UARTGetBuf(void);

uint8_t UARTGetLen(void);

uint8_t UARTStatus(void);

#endif

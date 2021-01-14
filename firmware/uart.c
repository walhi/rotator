#include <avr/io.h>
#include <avr/interrupt.h>
#include "uart.h"
#include "configure.h"

#define BAUD 9600
#define UBRR (F_CPU/16/BAUD - 1)

static char UARTBuf[16];
static char UARTBufPos;

extern struct config cfg;

uint8_t flag = 0;


char* UARTGetBuf()
{
  return UARTBuf;
}

uint8_t UARTGetLen()
{
  uint8_t val = UARTBufPos + 1;
  UARTBufPos = 0;
  return val;
}

void UARTInit()
{
	/* Set baud rate */
	UBRRH = (uint8_t)(UBRR>>8);
	UBRRL = (uint8_t)UBRR;
	/* Enable receiver and transmitter */
	UCSRB = (1 << RXEN) | (1 << TXEN) | (1 << RXCIE);
	/* Set frame format: 8 data, 1 stop bit */
	UCSRC = (1 << URSEL) | (3 << UCSZ0);

	UARTBufPos = 0;
}

void UARTSendChar(char data)
{
	/* Wait for empty transmit buffer */
	do {} while (!(UCSRA & (1 << UDRE)));
	/* Put data into buffer, sends the data */
	UDR = data;
}

void UARTSendBuf(char* data, uint8_t len)
{
	while (len--) UARTSendChar(*data++);
}

uint8_t UARTStatus(){
  if (flag){
    flag = 0;
    return 1;
  }
  return 0;
}

ISR(USART_RXC_vect)
{
	uint8_t data = UDR;



	UARTBuf[UARTBufPos++] = data;
	if (data == '\n' || data == '\r'){
    flag = 1;
		UARTBuf[UARTBufPos] = 0;
	}
	if (cfg.com_echo) UDR = data;
}

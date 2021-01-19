#include "uart.h"
#include "configure.h"

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
}

void UARTSendChar(char data)
{
  (void)data;
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

/*
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
*/

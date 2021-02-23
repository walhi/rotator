#include <8052.h>
#include "uart.h"
#include "configure.h"

/* Конфигурация */
extern struct config cfg;
//extern struct configAz cfgAz;
//extern struct configEl cfgEl;
//extern struct configFlags cfgFlags;

static char UARTRxBuf[8];
static char UARTRxBufPos;
static char UARTFIFOBuf[8];
static char UARTFIFOBufPos;
static char UARTFIFOBufPos2;
static char UARTTxLen = 0;
static uint8_t flag = 0;


char* UARTGetBuf()
{
  return UARTRxBuf;
}

uint8_t UARTGetLen()
{
  uint8_t val = UARTRxBufPos; // + 1;
  UARTRxBufPos = 0;
  return val;
}

/* Калькулятор константы для TH1 */
/* https://www.keil.com/products/c51/baudrate.asp */

void UARTInit(void)
{
	SCON = 0x50; /* Configure serial control register */
	PCON |= SMOD; /* SMOD bit set*/
	TMOD = 0x20; /* Using timer1,8-bit reload mode for baudrate generation */
	TH1 = 0xf3; /* 9600 baudrate */
	TR1 = 1; /* Start timer */
	EA  = 1;		/* Enable global interrupt */
	ES = 1;  		/* Enable serial interrupt */
}

void UARTSendChar(char data)
{
	if (UARTTxLen){
		UARTFIFOBuf[UARTFIFOBufPos2] = data;
		UARTTxLen++;
		UARTFIFOBufPos2++;
	} else {
		SBUF = data;
		UARTFIFOBufPos = 0;
		UARTFIFOBufPos2 = 0;
		UARTTxLen = 1;
	}
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

/* void Serial_ISR() __interrupt SI0_VECTOR */
void UARTInt()
{
	if (TI){
		TI = 0;
		UARTTxLen--;
		if (UARTTxLen){
			SBUF = UARTFIFOBuf[UARTFIFOBufPos];
			UARTFIFOBufPos++;
		}
	} else {
		RI = 0;			/* Clear RI flag */
		P2_0 = 0;
		uint8_t data = SBUF;
		UARTRxBuf[UARTRxBufPos++] = data;
		if (data == '\n' || data == '\r'){
			flag = 1;
			UARTRxBuf[UARTRxBufPos] = 0;
		}
		if (cfg.Flags.com_echo){
			UARTSendChar(data);
		}
	}
}

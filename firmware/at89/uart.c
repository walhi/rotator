#include <8052.h>
#include "uart.h"

char UARTBuf[16];
static char UARTBufPos;
static char UARTTxLen = 0;
static uint8_t flag = 0;

uint8_t UARTGetLen()
{
  uint8_t val = UARTBufPos; // + 1;
  UARTBufPos = 0;
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

void UARTSend(void)
{
  UARTBufPos = 0;
  SBUF = UARTBuf[UARTBufPos];
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
		if (UARTTxLen){
      if (UARTBuf[UARTBufPos]){
        SBUF = UARTBuf[UARTBufPos];
        UARTBufPos++;
      }
		}
	} else {
		RI = 0;			/* Clear RI flag */
		P2_0 = 0;
		uint8_t data = SBUF;
		UARTBuf[UARTBufPos++] = data;
		if (data == '\n' || data == '\r'){
			flag = 1;
			UARTBuf[UARTBufPos] = 0;
		}
	}
}

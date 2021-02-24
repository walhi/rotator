#include "gs232.h"
#include "uart.h"
#include "motors.h"

extern int16_t antAzimuth;
extern int16_t antElevation;
extern int16_t targetAzimuth;
extern int16_t targetElevation;



uint16_t parse(char* buf)
{
	uint16_t value = 0;
  value += (buf[0] - '0') * 100;
	value += (buf[1] - '0') * 10;
	value += buf[2] - '0';
  return value;
}

/* Protocol Yeasu GS-232
 * https://www.yaesu.com/downloadFile.cfm?FileID=820&FileCatID=155&FileName=GS232A.pdf&FileContentType=application%2Fpdf
 */
int8_t GS232Parse(uint8_t mode)
{
  char* buf = UARTGetBuf();
  uint8_t len = UARTGetLen();
  uint8_t d1, d2, d3;

	/* Go to start command (A-Z char) */
	while (len){
		if (buf[0] >= 'A' && buf[0] <= 'Z') break;
		buf++;
		len--;
	}

  if (!len || buf[0] == 0) return 0;

  if (mode == 0 && !(buf[0] == 'C' || buf[0] == 'B')) return 0;

	switch(buf[0]){
	case 'R':
		/* Start turning the rotator to the right */
		break; /* TODO check overlap*/
		motorAzStop();
		motorAzRight();
		break;
	case 'U':
		/* Start turning the rotator to the up */
		break; /* TODO check overlap*/
		motorElStop();
		motorElUp();
		break;
	case 'L':
		/* Start turning the rotator to the left */
		break; /* TODO check overlap*/
		motorAzStop();
		motorAzLeft();
		break;
	case 'D':
		/* Start turning the rotator to the down */
		break; /* TODO check overlap*/
		motorElStop();
		motorElDown();
		break;
	case 'A':
		/* Stop azimuth rotation */
		motorAzStop();
		break;
	case 'E':
		/* Stop elevation rotation */
		motorElStop();
		break;
	case 'S':
		/* Stop: cancel current command before completion. */
		targetAzimuth = antAzimuth;
		motorAzStop();
		motorElStop();
		break;
	case 'C':
		/* Return current azimuth angle in the form “+0nnn” degrees. */
    d1 = antAzimuth / 100 + '0';
    d2 = antAzimuth % 100 / 10 + '0';
    d3 = antAzimuth % 10 + '0';
    UARTSendChar('+');
    UARTSendChar('0');
    UARTSendChar(d1);
    UARTSendChar(d2);
    UARTSendChar(d3);
		if (buf[1] == '2'){
			/* Return azimuth and elevation (“+0aaa+0eee”) */
      UARTSendChar('+');
      UARTSendChar('0');
      UARTSendChar(antElevation / 100 + '0');
      UARTSendChar(antElevation % 100 / 10 + '0');
      UARTSendChar(antElevation % 10 + '0');
		}
    UARTSendChar('\r');
		break;
	case 'B':
		/* Return current elevation angle in the form “+0nnn” degrees. */
    UARTSendChar('+');
    UARTSendChar('0');
    UARTSendChar(antElevation / 100 + '0');
    UARTSendChar(antElevation % 100 / 10 + '0');
    UARTSendChar(antElevation % 10 + '0');
		UARTSendChar('\r');
    break;
	case 'M':
		/* Turn to aaa degrees azimuth. */
		targetAzimuth = parse(&buf[1]);
		return 1;
		break;
	case 'W':
		/* Turn to aaa degrees azimuth and eee degrees elevation. */
	default:
		/* Command not supported */
		break;
	}
	return 0;
}

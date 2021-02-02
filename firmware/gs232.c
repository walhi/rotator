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
void GS232Parse()
{
  char* buf = UARTGetBuf();
  uint8_t len = UARTGetLen();

	/* Go to start command (A-Z char) */
	while (len){
		if (buf[0] >= 'A' && buf[0] <= 'Z') break;
		buf++;
		len--;
	}

  if (!len || buf[0] == 0) return;

	switch(buf[0]){
	case 'R':
		/* Start turning the rotator to the right */
		motorAzStop();
		motorAzRight();
		break;
	case 'U':
		/* Start turning the rotator to the up */
		motorElStop();
		motorElUp();
		break;
	case 'L':
		/* Start turning the rotator to the left */
		motorAzStop();
		motorAzLeft();
		break;
	case 'D':
		/* Start turning the rotator to the down */
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
		motorAzStop();
		motorElStop();
		break;
	case 'C':
		/* Return current azimuth angle in the form “+0nnn” degrees. */
		if (buf[1] == '2'){
			/* Return azimuth and elevation (“+0aaa+0eee”) */

		}
		break;
	case 'B':
		/* Return current azimuth (elevation) angle in the form “+0nnn” degrees. */
		break;
	case 'M':
		/* Turn to aaa degrees azimuth. */
		targetAzimuth = parse(&buf[1]);
		break;
	case 'W':
		/* Turn to aaa degrees azimuth and eee degrees elevation. */
	default:
		/* Command not supported */
		break;
	}

}

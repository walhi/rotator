#include <stdlib.h>

#include "lcd.h"
#include "motors.h"
#include "ui.h"
#include "configure.h"
#include "encoder.h"
#include "uart.h"
#include "gs232.h"
#include "delay_hw.h"
#include "rotate_hw.h"

extern struct config cfg;

enum workMode mode;

extern int16_t antAzimuth;
int16_t portAzimuth = 0x7fff;

int16_t manAzimuth = 0x7fff;
int16_t targetAzimuth = 0x7fff;

int main (void)
{
  int16_t tmpAntAzimuth;
  int16_t tmpTargetAzimuth;
	int8_t step;
	uint8_t modeBtn;
  delay_hw_ms(50);

  motorsInit();
  encoderInit();
  LCDInit();
	UARTInit();

  cfg.az_count = 360;

	cfg.com_echo = 1;
  cfg.az_overlap_start = 170;
  cfg.az_overlap_end = 190;

  rotateHwInit();

  /* startupMessage(); */
	printUI();
  //configure();
  targetAzimuth = manAzimuth = portAzimuth = antAzimuth;
  while(1){
		modeBtn = encoderAzBtnGet();
    if (mode == port){
			if (modeBtn){
				motorAzStop();
				mode = manual;
				printUI();
				continue;
			}
			if (UARTStatus()) GS232Parse();
      if (targetAzimuth != portAzimuth){
        targetAzimuth = portAzimuth;
        printTarget();
      }
    } else if (mode == manual){
			if (modeBtn){
				motorAzStop();
				mode = port;
				printUI();
				continue;
			}
			step = encoderAzGet();
      if (step){
        targetAzimuth += step;
        printTarget();
      }
    }
    printAnt();
		continue;
    if (targetAzimuth != antAzimuth){
      /* Переход от 0 - 360 в -180 - 180 */
      tmpAntAzimuth = (antAzimuth > 180)?(antAzimuth - 360):antAzimuth;
      tmpTargetAzimuth = (targetAzimuth > 180)?(targetAzimuth - 360):targetAzimuth;
      /* Найдем путь */
      if (tmpAntAzimuth < tmpTargetAzimuth){
        /* Короткий путь при движении вправо */
        /* Проверка на перекручивание провода */

        motorAzStop();
        motorAzRight();
      } else {
        /* Короткий путь при движении влево */
        /* Движение влево */
        motorAzStop();
        motorAzLeft();
      }
    } else {
      motorAzStop();
    }
  }

  return 0;
}

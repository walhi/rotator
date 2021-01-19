#include <stdlib.h>
#include <8052.h>

#include "lcd.h"
#include "motors.h"
#include "encoder.h"
#include "delay_hw.h"

int main (void)
{
  LCDInit();

  LCDPrintString("Test AT89S52!");

  motorsInit();

  encoderInit();

	motorAzRight();
  while(1){

  }
  return 0;
}

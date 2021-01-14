#include <stdlib.h>

#include "motors.h"
#include "encoder.h"

int main (void)
{
  motorsInit();

  encoderInit();

	motorAzRight();
  while(1){

  }
  return 0;
}

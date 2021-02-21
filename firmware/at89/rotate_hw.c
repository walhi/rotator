#include <8052.h>
#include "rotate_hw.h"
#include "rotate.h"


void rotateHwInit()
{
  IT0 = 1;
  EX0 = 1;
}

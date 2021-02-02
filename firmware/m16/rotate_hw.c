#include <avr/interrupt.h>
#include "rotate_hw.h"
#include "rotate.h"
#include "signals.h"

void rotateHwInit()
{
  MCUCR |=  ((1 << ISC01) | (1 << ISC11));
  MCUCR |=  ((1 << ISC00) | (1 << ISC10));
  GICR  |= (1 << INT0) | (1 << INT1);
  GIFR  |= (1 << INTF0) | (1 << INTF1);

  MOTOR_AZ_EN_PORT_REG ^= MOTOR_AZ_EN_PIN;

  sei();
}


ISR(INT0_vect)
{
  azimuthImpulse();
}

ISR(INT1_vect)
{
  elevationImpulse();
}

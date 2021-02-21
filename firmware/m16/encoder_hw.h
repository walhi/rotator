#ifndef ENCODER_HW_H
#define ENCODER_HW_H

#include <stdint.h>
#include "signals.h"

#define TIMER_TIME 15

inline void encoderHwInit(void)
{
  ENC_AZ_A_DDR_REG &= ~ENC_AZ_A_PIN;
  ENC_AZ_A_PORT_REG |= ENC_AZ_A_PIN;

  ENC_AZ_B_DDR_REG &= ~ENC_AZ_B_PIN;
  ENC_AZ_B_PORT_REG |= ENC_AZ_B_PIN;

  ENC_AZ_BTN_DDR_REG &= ~ENC_AZ_BTN_PIN;
  ENC_AZ_BTN_PORT_REG |= ENC_AZ_BTN_PIN;

  ENC_EL_A_DDR_REG &= ~ENC_EL_A_PIN;
  ENC_EL_A_PORT_REG |= ENC_EL_A_PIN;

  ENC_EL_B_DDR_REG &= ~ENC_EL_B_PIN;
  ENC_EL_B_PORT_REG |= ENC_EL_B_PIN;

  ENC_EL_BTN_DDR_REG &= ~ENC_EL_BTN_PIN;
  ENC_EL_BTN_PORT_REG |= ENC_EL_BTN_PIN;
}

inline uint8_t encoderHwAzAGet(void)
{
  return (ENC_AZ_A_PIN_REG & ENC_AZ_A_PIN);
}

inline uint8_t encoderHwAzBGet(void)
{
  return (ENC_AZ_B_PIN_REG & ENC_AZ_B_PIN);
}

inline uint8_t encoderHwAzBtnGet(void)
{
  return (ENC_AZ_BTN_PIN_REG & ENC_AZ_BTN_PIN);
}

inline uint8_t encoderHwElAGet(void)
{
  return (ENC_EL_A_PIN_REG & ENC_EL_A_PIN);
}

inline uint8_t encoderHwElBGet(void)
{
  return (ENC_EL_B_PIN_REG & ENC_EL_B_PIN);
}

inline uint8_t encoderHwElBtnGet(void)
{
  return (ENC_EL_BTN_PIN_REG & ENC_EL_BTN_PIN);
}

void timerReload(void);

uint8_t timerStatus(void);


#endif

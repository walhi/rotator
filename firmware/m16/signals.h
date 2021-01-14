#ifndef SIGNALS_H
#define SIGNALS_H
#include <avr/io.h>

#define LCD_RS_PIN_REG PINA
#define LCD_RS_PORT_REG PORTA
#define LCD_RS_DDR_REG DDRA
#define LCD_RS_PIN (1 << PA0)

#define LCD_RW_PIN_REG PINA
#define LCD_RW_PORT_REG PORTA
#define LCD_RW_DDR_REG DDRA
#define LCD_RW_PIN (1 << PA1)

#define LCD_E_PIN_REG PINA
#define LCD_E_PORT_REG PORTA
#define LCD_E_DDR_REG DDRA
#define LCD_E_PIN (1 << PA2)

#define LCD_D4_PIN_REG PINA
#define LCD_D4_PORT_REG PORTA
#define LCD_D4_DDR_REG DDRA
#define LCD_D4_PIN (1 << PA3)

#define LCD_D5_PIN_REG PINA
#define LCD_D5_PORT_REG PORTA
#define LCD_D5_DDR_REG DDRA
#define LCD_D5_PIN (1 << PA4)

#define LCD_D6_PIN_REG PINA
#define LCD_D6_PORT_REG PORTA
#define LCD_D6_DDR_REG DDRA
#define LCD_D6_PIN (1 << PA5)

#define LCD_D7_PIN_REG PINA
#define LCD_D7_PORT_REG PORTA
#define LCD_D7_DDR_REG DDRA
#define LCD_D7_PIN (1 << PA6)

#define _PIN_REG PIN
#define _PORT_REG PORT
#define _DDR_REG DDR
#define _PIN (1 << P)

#define ENC_AZ_A_PIN_REG PINB
#define ENC_AZ_A_PORT_REG PORTB
#define ENC_AZ_A_DDR_REG DDRB
#define ENC_AZ_A_PIN (1 << PB1)

#define ENC_AZ_B_PIN_REG PINB
#define ENC_AZ_B_PORT_REG PORTB
#define ENC_AZ_B_DDR_REG DDRB
#define ENC_AZ_B_PIN (1 << PB2)

#define ENC_AZ_BTN_PIN_REG PINB
#define ENC_AZ_BTN_PORT_REG PORTB
#define ENC_AZ_BTN_DDR_REG DDRB
#define ENC_AZ_BTN_PIN (1 << PB0)

#define _PIN_REG PIN
#define _PORT_REG PORT
#define _DDR_REG DDR
#define _PIN (1 << P)

#define ENC_EL_A_PIN_REG PINB
#define ENC_EL_A_PORT_REG PORTB
#define ENC_EL_A_DDR_REG DDRB
#define ENC_EL_A_PIN (1 << PB1)

#define ENC_EL_B_PIN_REG PINB
#define ENC_EL_B_PORT_REG PORTB
#define ENC_EL_B_DDR_REG DDRB
#define ENC_EL_B_PIN (1 << PB2)

#define ENC_EL_BTN_PIN_REG PINB
#define ENC_EL_BTN_PORT_REG PORTB
#define ENC_EL_BTN_DDR_REG DDRB
#define ENC_EL_BTN_PIN (1 << PB0)

#define _PIN_REG PIN
#define _PORT_REG PORT
#define _DDR_REG DDR
#define _PIN (1 << P)

#define MOTOR_AZ_EN_PIN_REG PIND
#define MOTOR_AZ_EN_PORT_REG PORTD
#define MOTOR_AZ_EN_DDR_REG DDRD
#define MOTOR_AZ_EN_PIN (1 << PD6)

#define MOTOR_AZ_R1_PIN_REG PIND
#define MOTOR_AZ_R1_PORT_REG PORTD
#define MOTOR_AZ_R1_DDR_REG DDRD
#define MOTOR_AZ_R1_PIN (1 << PD5)

#define MOTOR_AZ_R2_PIN_REG PIND
#define MOTOR_AZ_R2_PORT_REG PORTD
#define MOTOR_AZ_R2_DDR_REG DDRD
#define MOTOR_AZ_R2_PIN (1 << PD4)

#define _PIN_REG PIN
#define _PORT_REG PORT
#define _DDR_REG DDR
#define _PIN (1 << P)

#define MOTOR_EL_EN_PIN_REG PIND
#define MOTOR_EL_EN_PORT_REG PORTD
#define MOTOR_EL_EN_DDR_REG DDRD
#define MOTOR_EL_EN_PIN (1 << PD3)

#define MOTOR_EL_R1_PIN_REG PIND
#define MOTOR_EL_R1_PORT_REG PORTD
#define MOTOR_EL_R1_DDR_REG DDRD
#define MOTOR_EL_R1_PIN (1 << PD2)

#define MOTOR_EL_R2_PIN_REG PIND
#define MOTOR_EL_R2_PORT_REG PORTD
#define MOTOR_EL_R2_DDR_REG DDRD
#define MOTOR_EL_R2_PIN (1 << PD1)

#endif

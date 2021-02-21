#include "i2c.h"

#include "delay_hw.h"

#define SCL_PIN P1_2
#define SDA_PIN P1_3
#define SDA_OUT {P1 |=  (1 << 3);}
#define SDA_IN  {P1 &= ~(1 << 3); SDA_PIN = 1;}

static void i2c_Clock(void);

void I2C_Init(void)
{
	P1 |= (1 << 2) | (1 << 3);
	SDA_PIN = 1;
	SCL_PIN = 1;

	/* Данный костыль нужен на случай */
	/* перезагрузки, когда 24c02 прижимает */
	/* линию SDA к земле. */
	uint8_t i = 8;
	while(i--){
		i2c_Clock();
		I2C_Stop();
	}
}

void I2C_Start(void)
{
	SDA_OUT;
	SDA_PIN = 1;
	SCL_PIN = 1;
	delay_hw_us(1);
	SDA_PIN = 0;
	delay_hw_us(1);
	SCL_PIN = 0;
}

void I2C_Stop(void)
{
	SCL_PIN = 1;
	delay_hw_us(1);
	SDA_OUT;
	SDA_PIN = 1;
}

uint8_t I2C_Write(uint8_t data)
{
	uint8_t i;
	SDA_OUT;
	for(i = 0; i < 8; i++){
		SDA_PIN = data & 0x80;
		i2c_Clock();
		data <<= 1;
	}

	// read ack
	SDA_IN;
	delay_hw_us(1);
	SCL_PIN = 1;
	delay_hw_us(1);
	i = SDA_PIN;
	SCL_PIN = 0;
	return i;
}

uint8_t I2C_Read(uint8_t ack)
{
	uint8_t i;
	uint8_t data = 0x00;

	SDA_IN;
	for(i = 0; i < 8; i++){
		delay_hw_us(1);
		SCL_PIN = 1;
		delay_hw_us(1);

		data <<= 1;
		data |= SDA_PIN;
		SCL_PIN = 0;
	}
	SDA_OUT;
	if(ack >= 1){
		SDA_PIN = 0;
	}	else {
		SDA_PIN = 1;
	}
	i2c_Clock();
	SDA_PIN = 1;

	return data;
}

static inline void i2c_Clock(void)
{
	delay_hw_us(1);
	SCL_PIN = 1;
	delay_hw_us(1);
	SCL_PIN = 0;
}

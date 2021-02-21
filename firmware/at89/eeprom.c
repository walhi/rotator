#include "eeprom.h"
#include "i2c.h"

#define EEPROM_ADDRESS 0b01010000

uint8_t getData(uint8_t* buf, uint8_t size, uint8_t addr)
{
	uint8_t ret = 0;

	/* Записываем адрес */
	I2C_Start();
	if (I2C_Write(EEPROM_ADDRESS << 1)){
		/* ACK бит не принят. EEPROM не обнаружена. */
		ret = 1;
	}
	if (I2C_Write(addr)){
		/* ACK бит не принят. EEPROM не обнаружена. */
		ret = 1;
	}
	I2C_Stop();

	/* Читаем данные */
	I2C_Start();
	if (I2C_Write(EEPROM_ADDRESS << 1 | 1)){
		/* ACK бит не принят. EEPROM не обнаружена. */
		ret = 3;
	}
	while(size){
		size--;
		*buf = I2C_Read(size);
		buf++;
	}


	I2C_Stop();
	return ret;
}

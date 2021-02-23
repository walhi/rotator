#include "eeprom.h"
#include "i2c.h"
#include "configure.h"
#include "delay_hw.h"

#define EEPROM_ADDRESS (uint8_t)(0b01010000 << 1)

extern struct config cfg;
extern int16_t antAzimuthPos;
extern int16_t antElevationPos;

static uint8_t posAddress;

static uint8_t readData(uint8_t* buf, uint8_t size, uint8_t addr)
{
	uint8_t ret = 0;

	/* Записываем адрес */
	I2C_Start();
	if (I2C_Write(EEPROM_ADDRESS)){
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
	if (I2C_Write(EEPROM_ADDRESS | 1)){
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


static uint8_t writeData(uint8_t* buf, uint8_t size, uint8_t addr)
{
	uint8_t ret = 0;

	/* Записываем адрес */
	I2C_Start();
	if (I2C_Write(EEPROM_ADDRESS)){
		/* ACK бит не принят. EEPROM не обнаружена. */
		ret = 1;
	}
	if (I2C_Write(addr)){
		/* ACK бит не принят. EEPROM не обнаружена. */
		ret = 1;
	}
	/* Посылаем данные */
	while(size){
		size--;
		if (I2C_Write(*buf)){
			/* ACK бит не принят. Ошибей. */
			ret = 3;
			break;
		}
		buf++;
	}
	I2C_Stop();
	return ret;
}

#define CONFIG_ADDRESS (uint8_t)(255 - sizeof(cfg) - 1)
uint8_t readConfig()
{
	uint8_t checksum = 0;
	uint8_t checksumReaded;
	uint8_t i = readData((uint8_t *)&cfg, sizeof(cfg), CONFIG_ADDRESS);
	if (i){
		/* ошибка чтения из eeprom */
		return i;
	}
	for (i = 0; i < sizeof(cfg); i++){
		checksum ^= ((uint8_t *)&cfg)[i];
	}
	readData(&checksumReaded, 1, 255);
	if (checksumReaded != checksum){
		return 0xff;
	}
	return 0;
}

void writeConfig()
{
	uint8_t checksum = 0;
	uint16_t i;
	for (i = 0; i < sizeof(cfg); i++){
		checksum ^= ((uint8_t *)&cfg)[i];
	}
	i = writeData((uint8_t *)&cfg, sizeof(cfg), CONFIG_ADDRESS);
	i = writeData((uint8_t *)&checksum, 1, 255);

}

void readAnt(void)
{
	/* Поиск ячейки с данными */
	posAddress = 0;
	/* TODO ротация */
	/* Ячейка найдена. Чтение позиции антенны. */
	readData((uint8_t *)&antAzimuthPos, sizeof(antAzimuthPos), posAddress);
	readData((uint8_t *)&antElevationPos, sizeof(antElevationPos), posAddress + 2);
}

void writeAnt(void)
{
	writeData((uint8_t *)&antAzimuthPos, sizeof(antAzimuthPos), posAddress);
	writeData((uint8_t *)&antElevationPos, sizeof(antElevationPos), posAddress + 2);
}

#ifndef __EEPROM_H__
#define __EEPROM_H__

#include "stm32f0xx_hal.h"

#define EEPROM_TIMEOUT        1000
#define EEPROM_UPTIME_ADDRESS 0x0010  // any safe offset

HAL_StatusTypeDef EEPROM_WriteByte(uint16_t address, uint8_t data);
HAL_StatusTypeDef EEPROM_ReadByte(uint16_t address, uint8_t *data);
HAL_StatusTypeDef EEPROM_WriteWord(uint16_t address, uint32_t data);
HAL_StatusTypeDef EEPROM_ReadWord(uint16_t address, uint32_t *data);

HAL_StatusTypeDef SaveUptimeToEEPROM(uint32_t uptime);
HAL_StatusTypeDef LoadUptimeFromEEPROM(uint32_t *uptime);
void EEPROM_Init(void);

#endif

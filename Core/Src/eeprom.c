#include "eeprom.h"
#include "main.h"
#include "stm32f0xx_hal.h"
#include "cmsis_os.h"

extern I2C_HandleTypeDef hi2c1;

// Optional mutex if you are multitasking
//extern osMutexId_t i2cMutexHandle;

// ---------------------------------------------------------------------------
// 🧩 Helper: Compute correct device address for 24C04
// ---------------------------------------------------------------------------
// 24C04 has two internal 256-byte blocks → A0 = memory bit 8
// Base address is 0x50 and A0 selects block 0x50 or 0x51
static inline uint8_t GetEEPROMAddress(uint16_t memAddr)
{
    return (uint8_t)((0x50 << 1) | ((memAddr >> 8) & 0x01));
}

// ---------------------------------------------------------------------------
// 💾 Low-Level Byte Read/Write (8-bit addressing)
// ---------------------------------------------------------------------------
HAL_StatusTypeDef EEPROM_WriteByte(uint16_t address, uint8_t data)
{
    uint8_t addr8 = address & 0xFF;
    uint8_t dev   = GetEEPROMAddress(address);

    HAL_StatusTypeDef status =
        HAL_I2C_Mem_Write(&hi2c1, dev, addr8, I2C_MEMADD_SIZE_8BIT, &data, 1, EEPROM_TIMEOUT);

    HAL_Delay(10);  // internal write cycle (tWR)
    return status;
}

HAL_StatusTypeDef EEPROM_ReadByte(uint16_t address, uint8_t *data)
{
    uint8_t addr8 = address & 0xFF;
    uint8_t dev   = GetEEPROMAddress(address);

    return HAL_I2C_Mem_Read(&hi2c1, dev, addr8, I2C_MEMADD_SIZE_8BIT, data, 1, EEPROM_TIMEOUT);
}

// ---------------------------------------------------------------------------
// 🧱 32-bit word read/write (4 bytes sequentially)
// ---------------------------------------------------------------------------
HAL_StatusTypeDef EEPROM_WriteWord(uint16_t address, uint32_t data)
{
    uint8_t dev   = GetEEPROMAddress(address);
    uint8_t addr8 = address & 0xFF;
    uint8_t buffer[4] = {
        (uint8_t)((data >> 24) & 0xFF),
        (uint8_t)((data >> 16) & 0xFF),
        (uint8_t)((data >> 8)  & 0xFF),
        (uint8_t)(data & 0xFF)
    };

    // ✅ Removed RTOS mutex calls
    HAL_StatusTypeDef status =
        HAL_I2C_Mem_Write(&hi2c1, dev, addr8, I2C_MEMADD_SIZE_8BIT, buffer, 4, EEPROM_TIMEOUT);

    HAL_Delay(10); // wait for write to finish
    return status;
}


HAL_StatusTypeDef EEPROM_ReadWord(uint16_t address, uint32_t *data)
{
    uint8_t dev   = GetEEPROMAddress(address);
    uint8_t addr8 = address & 0xFF;
    uint8_t buffer[4];

    HAL_StatusTypeDef status =
        HAL_I2C_Mem_Read(&hi2c1, dev, addr8, I2C_MEMADD_SIZE_8BIT, buffer, 4, EEPROM_TIMEOUT);

    if (status == HAL_OK)
    {
        *data = ((uint32_t)buffer[0] << 24) |
                ((uint32_t)buffer[1] << 16) |
                ((uint32_t)buffer[2] << 8)  |
                ((uint32_t)buffer[3]);
    }
    return status;
}

// ---------------------------------------------------------------------------
// 🕒 Application-Level Helpers (Uptime)
// ---------------------------------------------------------------------------
HAL_StatusTypeDef SaveUptimeToEEPROM(uint32_t uptime)
{
    return EEPROM_WriteWord(EEPROM_UPTIME_ADDRESS, uptime);
}

HAL_StatusTypeDef LoadUptimeFromEEPROM(uint32_t *uptime)
{
    return EEPROM_ReadWord(EEPROM_UPTIME_ADDRESS, uptime);
}

// ---------------------------------------------------------------------------
// 🧩 Initialization
// ---------------------------------------------------------------------------
extern volatile uint32_t systemTimeSec;
void EEPROM_Init(void)
{
    uint32_t loaded_time = 0;
    if (LoadUptimeFromEEPROM(&loaded_time) == HAL_OK)
    {
        if (loaded_time > 0 && loaded_time < 0xFFFFFFFF)
        {
            systemTimeSec = loaded_time;  // ✅ Restore saved uptime
        }
        else
        {
            systemTimeSec = 0;  // Invalid or blank EEPROM
        }
    }
    else
    {
        systemTimeSec = 0;  // Read failed
    }
}

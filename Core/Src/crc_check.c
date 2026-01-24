#include "stm32f4xx_hal.h"
#include "memory_map.h"

extern CRC_HandleTypeDef hcrc;

uint32_t calculate_crc(uint32_t start_addr, uint32_t size)
{
    uint32_t crc = 0xFFFFFFFF;
    uint32_t *data = (uint32_t*)start_addr;
    uint32_t length = size / 4;

    crc = HAL_CRC_Calculate(&hcrc, data, length);
    return crc;
}


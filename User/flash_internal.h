#ifndef __FLASH_INTERNAL_H__
#define __FLASH_INTERNAL_H__
#include "stm32f4xx_hal.h"
extern uint8_t flash_erase(uint8_t sector);
extern uint8_t flash_write(uint16_t *pbuf, uint8_t sector, uint32_t fw_size);
#endif // !__FLASH_INTERNAL_H__

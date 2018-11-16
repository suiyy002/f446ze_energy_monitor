#include "stm32f4xx_hal.h"
/**
 * @brief erase sector
 *
 * @param sector
 */
uint8_t flash_erase(uint8_t sector)
{
    static FLASH_EraseInitTypeDef EraseInitStruct;
    static uint32_t SECTORError = 0;
    HAL_FLASH_Unlock();
    // __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_PGSERR | FLASH_FLAG_WRPERR);
    EraseInitStruct.TypeErase     = FLASH_TYPEERASE_SECTORS;
    EraseInitStruct.VoltageRange  = FLASH_VOLTAGE_RANGE_3;
    EraseInitStruct.Sector        = sector;
    EraseInitStruct.NbSectors     = 1;
    if (HAL_FLASHEx_Erase(&EraseInitStruct, &SECTORError) != HAL_OK)
    {
        /*
            SECTORError will contain the faulty sector and then to know the code error on this sector,
            user can call function 'HAL_FLASH_GetError()'
        */
       return HAL_FLASH_GetError();
    }
    HAL_FLASH_Lock();
    return 0;
}

#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) /* Base address of Sector 0, 16 Kbytes */
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08004000) /* Base address of Sector 1, 16 Kbytes */
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08008000) /* Base address of Sector 2, 16 Kbytes */
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x0800C000) /* Base address of Sector 3, 16 Kbytes */
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08010000) /* Base address of Sector 4, 64 Kbytes */
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08020000) /* Base address of Sector 5, 128 Kbytes */
#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08040000) /* Base address of Sector 6, 128 Kbytes */
#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x08060000) /* Base address of Sector 7, 128 Kbytes */
static uint32_t GetSectorAddr(uint8_t sector)
{
    switch (sector)
    {
        case 0:
            return ADDR_FLASH_SECTOR_0;
        case 1:
            return ADDR_FLASH_SECTOR_1;
        case 2:
            return ADDR_FLASH_SECTOR_2;
        case 3:
            return ADDR_FLASH_SECTOR_3;
        case 4:
            return ADDR_FLASH_SECTOR_4;
        case 5:
            return ADDR_FLASH_SECTOR_5;
        case 6:
            return ADDR_FLASH_SECTOR_6;
        case 7:
            return ADDR_FLASH_SECTOR_7;
    }
		return 0;
}

uint8_t flash_write(uint16_t *pbuf, uint8_t sector, uint32_t size)
{
    if(flash_erase(sector))
        return 1;
    uint32_t total_words = size / 2;
    HAL_FLASH_Unlock();
    /* 开始写入字 */
    uint32_t current_addr = GetSectorAddr(sector); /* 获取sector开始地址 */
    while(total_words--)
    {
       if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, current_addr, *pbuf) == HAL_OK)
       {
           current_addr += 4;
           pbuf += 2; // 两个uint16_t
       }
       else
       {
           return 3;
       }
    }
    HAL_FLASH_Lock();
    return 0xff;
}
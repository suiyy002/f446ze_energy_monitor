#ifndef W25Q64_H_
#define W25Q64_H_

#include "stm32f4xx_hal.h"

//#define W25Q64_CS BIT0
//#define W25Q64_DO BIT1
//#define W25Q64_DI BIT2
//#define W25Q64_CLK BIT3
//#define W25Q64_VDD BIT4

/***************W25Q128*************************************
һ��BLOCK��64KB(����)
һ��SECTOR��4KB���飩
һ��128b/8=16MB
һ��16000KB/64KB=250��BLOCK
ÿ��BLOCK��64/8=8��sector




****************************************************/




#define SPIFlash_Set_DO    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_SET)//(P2OUT |= W25Q64_DI)
#define SPIFlash_Clr_DO    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_RESET)//(P2OUT &= ~W25Q64_DI)      

#define SPIFlash_Get_DI    HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_2)       //(P2IN &= W25Q64_DO)

#define SPIFlash_Set_SCLK    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, GPIO_PIN_SET)//(P2OUT |= W25Q64_CLK)
#define SPIFlash_Clr_SCLK    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, GPIO_PIN_RESET)//(P2OUT &= ~W25Q64_CLK)        
#define SPIFlash_Enable_CS   HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, GPIO_PIN_RESET)//(P2OUT &= ~W25Q64_CS)
#define SPIFlash_Disable_CS    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, GPIO_PIN_SET)//(P2OUT |= W25Q64_CS)        //P1.7
#define W25Q64_VDD_1    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, GPIO_PIN_SET)//(P2OUT |= W25Q64_VDD)
#define W25Q64_VDD_0    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, GPIO_PIN_RESET)//(P2OUT &= ~W25Q64_VDD)        //P1.6


//#define    SPIFlash_Enable_CS      (P8OUT &= 0XFE)           //��spiflashƬѡʹ��
//#define    SPIFlash_Disable_CS     (P8OUT |= 0X01)           //�ر�spiflashƬѡʹ��
//#define    SPIFlash_Set_SCLK       (P7OUT |= 0X08)           //����spiflashʱ����
//#define    SPIFlash_Clr_SCLK       (P7OUT &= 0Xf7)           //����spiflashʱ����
//#define    SPIFlash_Set_DO         (P8OUT |= 0X04)           //MOSI��������1,�����1�ߵ�ƽ
//#define    SPIFlash_Clr_DO         (P8OUT &= 0XFB)           //MOSI��������0,�����0�͵�ƽ



//typedef unsigned char		uint8_t;  /*  8 bits */
//typedef unsigned short int	uint16_t; /* 16 bits */
//typedef unsigned int	        uint32_t; /* 32 bits */

/******************************************************************************
** SPI_FLASH
********************************************************************************/
typedef enum
{
    RET_SUCCESS = 0,
    RET_FlashRead_ERROR,
    RET_WriteCmd_ERROR,
    RET_WaitBusy_ERROR
}SpiRetValueType;
/*****************************************************************************
** �ܽŶ���
*****************************************************************************/




#define    SPIFLASH_CMD_LENGTH        0x03
#define    SPIFLASH_WRITE_BUSYBIT     0X01
#define    SPIFlash_ReadData_CMD      0x03
#define    SPIFlash_WriteEnable_CMD   0x06
#define    SPIFlash_WriteDisable_CMD  0x04
#define    SPIFlash_PageProgram_CMD   0x02
#define    SPIFlash_WriteSR_CMD       0x01
#define    SPIFlash_ReadSR_CMD        0x05
#define    SPIFlash_SecErase_CMD      0x20
#define    SPIFlash_BlockErase_CMD    0xD8
#define    SPIFlash_PAGEBYTE_LENGTH   256
#define    SPIFlash_SECBYTE_LENGTH   (1024*4)

/*******************************************************************************
** ��������
*******************************************************************************/
uint8_t SpiFlash_ReadOneByte(void);
void SpiFlash_WriteOneByte(uint8_t DataBuffer);
uint8_t SpiFlash_Read(uint8_t *pBuffer,uint32_t ReadAddr,uint32_t ReadBytesNum);
uint8_t SpiFlash_Write_Page(uint8_t *pBuffer, uint32_t WriteAddr, uint32_t WriteBytesNum);
uint8_t SpiFlash_Write_MorePage(uint8_t *pBuffer, uint32_t WriteAddr, uint32_t WriteBytesNum);
uint8_t SpiFlash_Write_MoreSector(uint8_t *pBuffer, uint32_t WriteAddr, uint32_t WriteBytesNum);
uint16_t SPIFlash_GPIO_GetBit(void);
void SpiFlash_Write_Data(uint8_t *pBuffer,uint8_t Block_Num,uint8_t Page_Num,uint32_t WriteBytesNum);
void SpiFlash_Read_Data(uint8_t *pBuffer,uint8_t Block_Num ,uint8_t Page_Num ,uint32_t ReadBytesNum);
void SPIFlash_Erase_Block(uint8_t BlockNum);

 void w25q64_init(void);

void SpiFlash_Write_Sector_Data(uint8_t *pBuffer,uint8_t Block_Num,uint8_t Sector_Num,uint32_t WriteBytesNum);
void SpiFlash_Read_Sector_Data(uint8_t *pBuffer,uint8_t Block_Num ,uint8_t Sector_Num,uint32_t ReadBytesNum);
void SPIFlash_Erase_Sector(uint8_t Block_Num,uint8_t Sector_Number);

#endif

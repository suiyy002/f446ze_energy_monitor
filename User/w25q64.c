#include "stm32f4xx_hal.h"  
#include "w25q64.h"




void w25q64_init(void)
{
//P2SEL&=~(BIT0+BIT1+BIT2+BIT3+BIT4);
//P2DIR|=BIT0+BIT2+BIT3+BIT4;
//P2DIR&=~BIT1;
W25Q64_VDD_1;
}


/*
void w25q64_delay(uint16_t k)
{
for(uint16_t i=0;i<k;k++)
{ 
    ;
}
}*/

/*****************************************************************************
** �ļ����ƣ�uint8_t SpiFlash_ReadOneByte(void)
** ��    �ܣ�
** �޸���־��
******************************************************************************/

uint8_t SpiFlash_ReadOneByte(void)
{
    uint8_t BitCount = 0;
    uint8_t retValue = 0;
    SPIFlash_Set_SCLK;            //ʱ��������,�ָ�ʱ����Ϊ�ߵ�ƽ
   //  w25q64_delay(10);
    for(BitCount = 0;BitCount < 8; BitCount++)
    {   //  w25q64_delay(10);
        retValue <<= 1;
        //     w25q64_delay(10);
        SPIFlash_Set_SCLK;            //ʱ��������,�ָ�ʱ����Ϊ�ߵ�ƽ
        //     w25q64_delay(10);
        if(SPIFlash_Get_DI)
        {
            retValue |= 0x01;
        }
        else
        {
            retValue &= 0xFE;
        }
         //    w25q64_delay(10);
        SPIFlash_Clr_SCLK;            //ʱ��������,�����½��ض�������
    }
        // w25q64_delay(10);
    SPIFlash_Set_SCLK;
    return (retValue);
}

/*****************************************************************************
** �ļ����ƣ�void SpiFlash_WriteOneByte(uint8_t DataBuffer)
** ��    �ܣ�
** �޸���־��
******************************************************************************/

void SpiFlash_WriteOneByte(uint8_t DataBuffer)
{
    uint8_t BitCount = 0;
    SPIFlash_Clr_SCLK;          //ʱ�������ͣ��ָ�ʱ����Ϊ�͵�ƽ
       //  w25q64_delay(10);
    for(BitCount = 0;BitCount < 8; BitCount++)
    {    //    w25q64_delay(10);
        SPIFlash_Clr_SCLK;          //ʱ�������ͣ��ָ�ʱ����Ϊ�͵�ƽ
        if(DataBuffer & 0x80)
        {    //   w25q64_delay(10);
            SPIFlash_Set_DO;
        }
        else
        {    // w25q64_delay(10);
            SPIFlash_Clr_DO;
        }
            // w25q64_delay(10);
        DataBuffer <<= 1;
            // w25q64_delay(10);
        SPIFlash_Set_SCLK;            //ʱ�������ߣ�����������д������
           //  w25q64_delay(10);
    }
    SPIFlash_Clr_SCLK;
        // w25q64_delay(10);
    SPIFlash_Set_DO;                //һ�ֽ����ݴ�����ϣ�MOSI�������ø߱�ʾ����״̬

}

/*****************************************************************************
** �ļ����ƣ�static uint8_t SpiFlash_Write_CMD(uint8_t *CMD)
** ��    �ܣ�
** �޸���־��
******************************************************************************/

static uint8_t SpiFlash_Write_CMD(uint8_t *CMD)
{
    uint32_t i = 0;
    for(i = 0;i < SPIFLASH_CMD_LENGTH; i++)
    {
        SpiFlash_WriteOneByte(CMD[i]);  //��SD����Ƭѡ�źŲ�д����������
    }

    return RET_SUCCESS;
}

/*****************************************************************************
** �ļ����ƣ�static uint8_t SpiFlash_ReadSR(void)
** ��    �ܣ�
** �޸���־��
******************************************************************************/

 uint8_t SpiFlash_ReadSR(void)
{
    uint8_t retValue = 0;
    SPIFlash_Enable_CS;
    SpiFlash_WriteOneByte(SPIFlash_ReadSR_CMD);
    retValue = SpiFlash_ReadOneByte();
    SPIFlash_Disable_CS;
    return retValue;
}


/*****************************************************************************
** �ļ����ƣ�static uint8_t SpiFlash_Wait_Busy(void)
** ��    �ܣ�
** �޸���־��
******************************************************************************/

static uint8_t SpiFlash_Wait_Busy(void)
{
   // sDelayT.cDelay_Count=0;
    //while((SpiFlash_ReadSR()&SPIFLASH_WRITE_BUSYBIT) == 0X01)
    //{  
      // if(sDelayT.cDelay_Count==100)
       //   break;     
   // }
    return RET_SUCCESS;
}

/*****************************************************************************
** �ļ����ƣ�uint8_t SpiFlash_Read(uint8_t *pBuffer,uint32_t ReadAddr,uint32_t ReadBytesNum)
** ��    �ܣ�
** �޸���־��
******************************************************************************/

uint8_t SpiFlash_Read(uint8_t *pBuffer,uint32_t ReadAddr,uint32_t ReadBytesNum)
{
    uint32_t i = 0;
    uint8_t pcmd[SPIFLASH_CMD_LENGTH] = {0};
    uint8_t ret = 0;

    SPIFlash_Enable_CS;              //��spiflashƬѡ

    SpiFlash_WriteOneByte(SPIFlash_ReadData_CMD);

    pcmd[0] = (uint8_t)((ReadAddr&0x00ff0000)>>16);
    pcmd[1] = (uint8_t)((ReadAddr&0x0000ff00)>>8);
    pcmd[2] = (uint8_t)ReadAddr;
    ret = SpiFlash_Write_CMD(pcmd);
    if(ret != RET_SUCCESS)
    {
        return RET_WriteCmd_ERROR;
    }

    for(i = 0;i < ReadBytesNum; i++)
    {
       pBuffer[i] = SpiFlash_ReadOneByte();  //��ȡSPIflash��ָ��bytes�ֽ�����

    }
    SPIFlash_Disable_CS;
    return RET_SUCCESS;
}

/*****************************************************************************
** �ļ����ƣ�static void SpiFlash_Write_Enable(void)
** ��    �ܣ�
** �޸���־��
******************************************************************************/

static void SpiFlash_Write_Enable(void)
{
    SPIFlash_Enable_CS;
        // w25q64_delay(10);
    SpiFlash_WriteOneByte(SPIFlash_WriteEnable_CMD);
        // w25q64_delay(10);
    SPIFlash_Disable_CS;
}

/*****************************************************************************
** �ļ����ƣ� void SPIFlash_Erase_Sector(uint8_t Block_Num,uint8_t Sector_Number)
** ��    �ܣ�
** �޸���־��
******************************************************************************/

void SPIFlash_Erase_Sector(uint8_t Block_Num,uint8_t Sector_Number)
{
    uint8_t pcmd[3] = {0};
    SpiFlash_Write_Enable();
    SPIFlash_Enable_CS;
    SpiFlash_WriteOneByte(SPIFlash_SecErase_CMD);

    pcmd[0] = Block_Num;
    pcmd[1] = Sector_Number<<4;
    pcmd[2] = 0;
    SpiFlash_Write_CMD(pcmd);
    SPIFlash_Disable_CS;
    SpiFlash_Wait_Busy();    //ÿ�β������ݶ�Ҫ��ʱ�ȴ�д�����

    return ;
}


/*****************************************************************************
** �ļ����ƣ�uint8_t SpiFlash_Write_Page(uint8_t *pBuffer, uint32_t WriteAddr, uint32_t WriteBytesNum)
** ��    �ܣ�
** �޸���־��
******************************************************************************/
uint8_t SpiFlash_Write_Page(uint8_t *pBuffer, uint32_t WriteAddr, uint32_t WriteBytesNum)
{
    uint32_t i = 0;
    uint8_t pcmd[SPIFLASH_CMD_LENGTH] = {0};
    uint8_t ret = 0;

    SpiFlash_Write_Enable();
    SPIFlash_Enable_CS;
    SpiFlash_WriteOneByte(SPIFlash_PageProgram_CMD);
    pcmd[0] = (uint8_t)((WriteAddr&0x00ff0000)>>16);
    pcmd[1] = (uint8_t)((WriteAddr&0x0000ff00)>>8);
    pcmd[2] = (uint8_t)WriteAddr;
    ret = SpiFlash_Write_CMD(pcmd);

    if(ret != RET_SUCCESS)
    {
        return RET_WriteCmd_ERROR;
    }

    for(i = 0;i < WriteBytesNum; i++)
    {

        SpiFlash_WriteOneByte(pBuffer[i]);  //��SPIflash��д�����һҳ256bytes�ֽ�����
    }
    SPIFlash_Disable_CS;
    ret = SpiFlash_Wait_Busy();                   //ÿ��д��һ�����������ݶ�Ҫ��ʱ�ȴ�д�����
    if(ret != RET_SUCCESS)
        return RET_WaitBusy_ERROR;

    return RET_SUCCESS;
}


/*****************************************************************************
** �ļ����ƣ�uint8_t SpiFlash_Write_MorePage(uint8_t *pBuffer, uint32_t WriteAddr, uint32_t WriteBytesNum)
** ��    �ܣ�
** �޸���־��
******************************************************************************/
uint8_t SpiFlash_Write_MorePage(uint8_t *pBuffer, uint32_t WriteAddr, uint32_t WriteBytesNum)
{

    uint16_t PageByteRemain = 0;
    PageByteRemain = SPIFlash_PAGEBYTE_LENGTH - WriteAddr%SPIFlash_PAGEBYTE_LENGTH;
    if(WriteBytesNum <= PageByteRemain)
    {
        PageByteRemain = WriteBytesNum;
    }
    while(1)
    {
        SpiFlash_Write_Page(pBuffer,WriteAddr,PageByteRemain);
        if(WriteBytesNum == PageByteRemain)
        {
            break;
        }
        else
        {
            pBuffer += PageByteRemain;
            WriteAddr += PageByteRemain;
            WriteBytesNum -= PageByteRemain;
            if(WriteBytesNum > SPIFlash_PAGEBYTE_LENGTH)
            {
                PageByteRemain = SPIFlash_PAGEBYTE_LENGTH;
            }
            else
            {
                PageByteRemain = WriteBytesNum;
            }
        }
    }
    return RET_SUCCESS;
}

//
///*****************************************************************************
//** �ļ����ƣ�uint8_t SpiFlash_Write_MoreSector(uint8_t *pBuffer, uint32_t WriteAddr, uint32_t WriteBytesNum)
//** ��    �ܣ�
//** �޸���־��
//******************************************************************************/
//uint8_t SpiFlash_Write_MoreSector(uint8_t *pBuffer, uint32_t WriteAddr, uint32_t WriteBytesNum)
//{
//    uint32_t Secpos = 0;
//    uint16_t Secoff = 0;
//    uint16_t Secremain = 0;
//    uint32_t i = 0;
//    uint8_t  SPIBufferTemp[SPIFlash_SECBYTE_LENGTH] = {0};
//    Secpos = WriteAddr/SPIFlash_SECBYTE_LENGTH;
//    Secoff = WriteAddr%SPIFlash_SECBYTE_LENGTH;
//    Secremain = SPIFlash_SECBYTE_LENGTH - Secoff;
//    if(WriteBytesNum <= Secremain)
//    {
//        Secremain = WriteBytesNum;
//    }
//
//    while(1)
//    {
//        if(SpiFlash_Read(SPIBufferTemp,Secpos*SPIFlash_SECBYTE_LENGTH,SPIFlash_SECBYTE_LENGTH) != RET_SUCCESS)
//        {
//             return RET_FlashRead_ERROR;
//        }
//        for(i = 0;i < Secremain;i++)
//        {
//            if(SPIBufferTemp[Secoff+i] != 0xFF)
//                break;                  //��Ҫ����һ������
//        }
//        if(i < Secremain)               //��Ҫ����һ������
//        {
//            SPIFlash_Erase_Sector(Secpos);      //�����������
//            for(i = 0;i < Secremain;i++)
//            {
//                SPIBufferTemp[i+Secoff] = pBuffer[i];
//            }
//            SpiFlash_Write_MorePage(SPIBufferTemp,Secpos*SPIFlash_SECBYTE_LENGTH,SPIFlash_SECBYTE_LENGTH);
//        }
//        else
//        {
//            SpiFlash_Write_MorePage(pBuffer,WriteAddr,Secremain);
//        }
//
//        if(WriteBytesNum == Secremain)
//        {
//            break;
//        }
//        else
//        {
//            Secpos++;
//            Secoff = 0;
//            pBuffer += Secremain;
//            WriteAddr += Secremain;
//            WriteBytesNum -=Secremain;
//            if(WriteBytesNum > SPIFlash_SECBYTE_LENGTH)
//            {
//                Secremain = SPIFlash_SECBYTE_LENGTH;
//            }
//            else
//            {
//                Secremain = WriteBytesNum;
//            }
//        }
//    }
//    return RET_SUCCESS;
//}
//

/*****************************************************************************
** �ļ����ƣ�uint16_t SPIFlash_GPIO_GetBit(void)
** ��    �ܣ�
** �޸���־��
******************************************************************************/
uint16_t SPIFlash_GPIO_GetBit(void)
{
  uint16_t retValue = 0;
  uint16_t valueTemp = 0;
  valueTemp = SPIFlash_Get_DI;
  retValue = (valueTemp &=0x01)>>0x01;            //BIT1����0x02
  return retValue;
}


/*****************************************************************************
** �ļ����ƣ�void SPIFlash_Erase_Block(uint8_t BlockNum)
** ��    �ܣ�����BLOCK
** �޸���־��
******************************************************************************/

void SPIFlash_Erase_Block(uint8_t BlockNum)
{
    uint8_t pcmd[3] = {0};

    SpiFlash_Write_Enable();   //дʹ��
    SPIFlash_Enable_CS;        //Ƭѡ����
    SpiFlash_WriteOneByte(SPIFlash_BlockErase_CMD);  //����Block����ָ��
    pcmd[0] =BlockNum ;        //��24λ��ַ ��Ϊ��Ƭ��Ϊ16λ ���Է����δ�
    SpiFlash_Write_CMD(pcmd);
    SPIFlash_Disable_CS;
    SpiFlash_Wait_Busy();                   //ÿ�β������ݶ�Ҫ��ʱ�ȴ�д�����

    return ;
}

/*****************************************************************************
** �ļ����ƣ�uint8_t SpiFlash_Write_Data(uint8_t *pBuffer,uint8_t Block_Num,int8_t Page_Num,uint32_t WriteBytesNum)
** ��    �ܣ�д�ĸ�������һҳ������
** �޸���־��
******************************************************************************/
void SpiFlash_Write_Data(uint8_t *pBuffer,uint8_t Block_Num,uint8_t Page_Num,uint32_t WriteBytesNum)
{
    
    uint8_t pcmd[3] = {0};
    
    SpiFlash_Write_Enable();
    SPIFlash_Enable_CS;
    SpiFlash_WriteOneByte(SPIFlash_PageProgram_CMD);
    pcmd[0] = Block_Num;
    pcmd[1] = Page_Num;
    pcmd[2] = 0;
    SpiFlash_Write_CMD(pcmd);

    for(uint32_t i = 0;i < WriteBytesNum; i++)
    {
        SpiFlash_WriteOneByte(pBuffer[i]);  //��SPIflash��д�����һҳ256bytes�ֽ�����
    }
    SPIFlash_Disable_CS;
    SpiFlash_Wait_Busy();                   //ÿ��д��һ�����������ݶ�Ҫ��ʱ�ȴ�д�����
    return ;
}
/*****************************************************************************
** �ļ����ƣ�void SpiFlash_Write_Sector_Data(uint8_t *pBuffer,uint8_t Block_Num,int8_t Sector_Num,uint32_t WriteBytesNum)
** ��    �ܣ�д�ĸ������ĸ�sector�е�����
** �޸���־��
******************************************************************************/
void SpiFlash_Write_Sector_Data(uint8_t *pBuffer,uint8_t Block_Num,uint8_t Sector_Num,uint32_t WriteBytesNum)
{
   uint8_t Write_Page_Num=0;

   Write_Page_Num = WriteBytesNum/256;
   if((WriteBytesNum%256)!=0) Write_Page_Num+=1;
   for(uint8_t i=0;i<Write_Page_Num;i++)
   {    
     SpiFlash_Write_Data(pBuffer,Block_Num,i+(Sector_Num<<4),256);
     pBuffer=pBuffer+256;   
   }
   return ;
}

/*****************************************************************************
** �ļ����ƣ�void SpiFlash_Read_Sector_Data(uint8_t *pBuffer,uint8_t Block_Num ,uint8_t Sector_Num ,uint32_t ReadBytesNum)
** ��    �ܣ�
** �޸���־��
******************************************************************************/

void SpiFlash_Read_Sector_Data(uint8_t *pBuffer,uint8_t Block_Num ,uint8_t Sector_Num,uint32_t ReadBytesNum)
{
    uint32_t i = 0;
    uint8_t pcmd[SPIFLASH_CMD_LENGTH] = {0};

    SPIFlash_Enable_CS;              //��spiflashƬѡ
    SpiFlash_WriteOneByte(SPIFlash_ReadData_CMD);
    pcmd[0] =Block_Num ;
    pcmd[1] =Sector_Num<<4;
    pcmd[2] = 0;
    SpiFlash_Write_CMD(pcmd);

    for(i = 0;i < ReadBytesNum; i++)
    {
       pBuffer[i] = SpiFlash_ReadOneByte();  //��ȡSPIflash��ָ��bytes�ֽ�����

    }
    SPIFlash_Disable_CS;
    
    return ;
}

/*****************************************************************************
** �ļ����ƣ�uint8_t SpiFlash_Read_Data(uint8_t *pBuffer,uint8_t Block_Num ,uint8_t Page_Num ,uint32_t ReadBytesNum)
** ��    �ܣ�
** �޸���־��
******************************************************************************/

void SpiFlash_Read_Data(uint8_t *pBuffer,uint8_t Block_Num ,uint8_t Page_Num ,uint32_t ReadBytesNum)
{
    
    uint8_t pcmd[SPIFLASH_CMD_LENGTH] = {0};

    SPIFlash_Enable_CS;              //��spiflashƬѡ
    SpiFlash_WriteOneByte(SPIFlash_ReadData_CMD);
    pcmd[0] =Block_Num ;
    pcmd[1] =Page_Num ;
    pcmd[2] = 0;
    SpiFlash_Write_CMD(pcmd);

    for(uint32_t i = 0 ;i < ReadBytesNum; i++)
    {
       pBuffer[i] = SpiFlash_ReadOneByte();  //��ȡSPIflash��ָ��bytes�ֽ�����

    }
    SPIFlash_Disable_CS;
    return ;
}

/*******************************************************************************
** �������ƣ�void Spi_Flash_init(void)
** ��    �ܣ�
** �޸���־��
*******************************************************************************/


/*****************************************
���ܣ�NRF24L01��Ƶģ��ͷ�ļ�
*****************************************/
#ifndef _NRF24L01_H_
#define _NRF24L01_H_
/***************************************************/
typedef unsigned char     BYTE;

#define uchar unsigned char

//24L01���ͽ������ݿ�ȶ���
#define TX_ADR_WIDTH    5   //5�ֽڵĵ�ַ���
#define RX_ADR_WIDTH    5   //5�ֽڵĵ�ַ���
#define TX_PLOAD_WIDTH  32  //32�ֽڵ��û����ݿ��
#define RX_PLOAD_WIDTH  32  //32�ֽڵ��û����ݿ��


#define nRF_TX_Mode	0x00
#define nRF_RX_Mode	0x01
///****************************************************************//
// SPI(nRF24L01) commands
#define RF_READ_REG        	0x00  // Define read command to register
#define RF_WRITE_REG       	0x20  // Define write command to register
#define RD_RX_PLOAD	0x61  // Define RX payload register address
#define WR_TX_PLOAD  0xA0  // Define TX payload register address
#define FLUSH_TX        	0xE1  // Define flush TX register command
#define FLUSH_RX        	0xE2  // Define flush RX register command
#define REUSE_TX_PL    0xE3  // Define reuse TX payload register command
#define NOP             	0xFF  // Define No Operation, might be used to read status register

//***************************************************//
// SPI(nRF24L01) registers(addresses)
#define CONFIG          	0x00  // 'Config' register address
#define EN_AA           	0x01  // 'Enable Auto Acknowledgment' register address
#define EN_RXADDR       0x02  // 'Enabled RX addresses' register address
#define SETUP_AW        	0x03  // 'Setup address width' register address
#define SETUP_RETR      0x04  // 'Setup Auto. Retrans' register address
#define RF_CH           	0x05  // 'RF channel' register address
#define RF_SETUP        	0x06  // 'RF setup' register address
#define STATUS          	0x07  // 'Status' register address
#define OBSERVE_TX      0x08  // 'Observe TX' register address
#define CD              		0x09  // 'Carrier Detect' register address
#define RX_ADDR_P0	0x0A  // 'RX address pipe0' register address
#define RX_ADDR_P1	0x0B  // 'RX address pipe1' register address
#define RX_ADDR_P2	0x0C  // 'RX address pipe2' register address
#define RX_ADDR_P3	0x0D  // 'RX address pipe3' register address
#define RX_ADDR_P4	0x0E  // 'RX address pipe4' register address
#define RX_ADDR_P5	0x0F  // 'RX address pipe5' register address
#define TX_ADDR         	0x10  // 'TX address' register address
#define RX_PW_P0        	0x11  // 'RX payload width, pipe0' register address
#define RX_PW_P1        	0x12  // 'RX payload width, pipe1' register address
#define RX_PW_P2        	0x13  // 'RX payload width, pipe2' register address
#define RX_PW_P3        	0x14  // 'RX payload width, pipe3' register address
#define RX_PW_P4        	0x15  // 'RX payload width, pipe4' register address
#define RX_PW_P5        	0x16  // 'RX payload width, pipe5' register address
#define FIFO_STATUS	0x17  // 'FIFO Status Register' register address
//========================
/******   STATUS�Ĵ���bitλ����      *******/
#define MAX_RT  	0x10  //�ﵽ����ʹ����ж�
#define TX_OK   	0x20  //TX��������ж�
#define RX_OK   	0x40  //���յ������ж�
/**************************************************/

///===============REG setting data===============
//===0x00 CONFIG===============
#define MASK_RX_DR	0x40
#define MASK_TX_DS	0x20
#define MASK_MAX_RT	0x10
#define EN_CRC		0x08
#define CRCO			0x04// 0---1byte,1---2byte
#define PWR_UP		0x02// 1---power up,0---power down
#define PRIM_RX		0x01// 1---RX mode,0---TX mode
//===0x01 EN_AA================
//ʹ������Ӧͨ����Ĭ��5ͨ��ȫ��
#define ENAA_P5		0x20
#define ENAA_P4		0x10
#define ENAA_P3		0x08
#define ENAA_P2		0x04
#define ENAA_P1		0x02
#define ENAA_P0		0x01
#define ENAA_DisableALL	0x00
//===0x02 EN_RXADDR============
#define ERX_P5		0x20
#define ERX_P4		0x10
#define ERX_P3		0x08
#define ERX_P2		0x04
#define ERX_P1		0x02
#define ERX_P0		0x01
#define ERX_None		0x00
//===0x03 SETUP_AW=============
#define AW_3Bytes		0x01
#define AW_4Bytes		0x02
#define AW_5Bytes		0x03
//===0x04 SETUP_RETR============
#define AutoReTxDalay_250uS	0x00//default
#define AutoReTxDalay_500uS	0x10
#define AutoReTxDalay_750uS	0x20
#define AutoReTxDalay_1000uS	0x30
#define AutoReTxDalay_1250uS	0x40
#define AutoReTxDalay_1500uS	0x50
#define AutoReTxDalay_1750uS	0x60
#define AutoReTxDalay_2000uS	0x70
#define AutoReTxDalay_2250uS	0x80
#define AutoReTxDalay_2500uS	0x90
#define AutoReTxDalay_2750uS	0xA0
#define AutoReTxDalay_3000uS	0xB0
#define AutoReTxDalay_3250uS	0xC0
#define AutoReTxDalay_3500uS	0xD0
#define AutoReTxDalay_3750uS	0xE0
#define AutoReTxDalay_4000uS	0xF0
#define AutoReTx_Disable		0x00
#define AutoReTxTimes_1		0x01
#define AutoReTxTimes_2		0x02
#define AutoReTxTimes_3		0x03//default
#define AutoReTxTimes_4		0x04
#define AutoReTxTimes_5		0x05
#define AutoReTxTimes_6		0x06
#define AutoReTxTimes_7		0x07
#define AutoReTxTimes_8		0x08
#define AutoReTxTimes_9		0x09
#define AutoReTxTimes_10		0x0A
#define AutoReTxTimes_11		0x0B
#define AutoReTxTimes_12		0x0C
#define AutoReTxTimes_13		0x0D
#define AutoReTxTimes_14		0x0E
#define AutoReTxTimes_15		0x0F
//===0x06 RF_SETUP============
#define RF_DR_1Mbps			0x00
#define RF_DR_2Mbps			0x08
//#define RF_PWR_


unsigned char SPI_RW(uchar byte,unsigned char W_RF);
unsigned char SPI_RW_Reg(BYTE reg, BYTE value,unsigned char W_RF);
BYTE SPI_Read(BYTE reg,unsigned char W_RF);
unsigned char SPI_Read_Buf(BYTE reg, BYTE *pBuf, BYTE bytes,unsigned char W_RF);
unsigned char SPI_Write_Buf(BYTE reg, BYTE const *pBuf, BYTE bytes,unsigned char W_RF);
void RX_Mode(unsigned char W_RF);
void TX_Mode(unsigned char W_RF);
void nRF24L01_Init(unsigned char Mode,unsigned char W_RF);
void nRF24L01_Send(unsigned char W_RF);
void nRF24L01_Revceive(unsigned char W_RF);
uchar NRF_Check(unsigned char W_RF);


#endif

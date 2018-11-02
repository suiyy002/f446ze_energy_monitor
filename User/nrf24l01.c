#include "stm32f4xx_hal.h"
#include "nrf24l01.h"
#include "nrf24l01_reg.h"
#include "delay.h"

unsigned char TestBuf=0;


uchar  const TX_ADDRESS[TX_ADR_WIDTH]={0xFF,0xFF,0xFF,0xFF,0xFF}; // Define a static TX address
uchar  const TX_ADDRESS1[TX_ADR_WIDTH]={0xed,0xed,0xed,0xed,0xed};
uchar  const TX_ADDRESS2[TX_ADR_WIDTH]={0xe1,0xe1,0xe1,0xe1,0xe1};
uchar  const TX_ADDRESS3[TX_ADR_WIDTH]={0xFF,0xFF,0xFF,0xFF,0xFF};

uchar NRF_rx_buf[RX_PLOAD_WIDTH];  
uchar NRF_rx_buf1[RX_PLOAD_WIDTH];  
uchar NRF_rx_buf2[RX_PLOAD_WIDTH];  
//uchar tx_buf[]={0x1c,0xc9,0xcf,0xba,0xa3,0xb1,0xa6,0xc7,0xb6,0xb5,0xe7,0xd7,0xd3};
uchar tx_buf[]={0x19,0x89,0xc9,0x12,0xc9,0x11,0xc9,0x23,0xc9,0x1d,0xc9,0x3d,0xc9,0x34};
uchar flag,q;



//extern TIM_HandleTypeDef htim4;
//extern timerstandard Timerstandard;



//timfalg  TimeFalg;

//******************************************************************************************
//延时函数
//******************************************************************************************

void delay_50us(unsigned int i)
{
	//for(unsigned char i=0; i<77; i++);
	unsigned long j;
	for(;i>0;i--)
	{
		for(j=0;j<33;j++);//50.4us
	}
}

void delay_100ms(unsigned int i)
{
	delay_ms(i);
}

void msDelay(unsigned int ms)
{
   delay_ms(ms);
}
void delay_1ms(unsigned int i)
{
	 delay_ms(i);
}
void inerDelay_us(int n)
{
	for(;n>0;n--);
}

//***********************************************************************************
void CE_Pin(BYTE state,unsigned char W_RF)
{
  switch(W_RF)
  {case 0 :
    if(state)
    nRF24L01_CE_1;
  else
    nRF24L01_CE_0;  break;
  case 1 :
    if(state)
    nRF24L01_CE1_1;
  else
    nRF24L01_CE1_0;  break;
    case 2 :
    if(state)
    nRF24L01_CE2_1;
  else
    nRF24L01_CE2_0;  break;
    case 3 :
    if(state)
    nRF24L01_CE3_1;
  else
    nRF24L01_CE3_0;  break;
    
     default:  break;
    
    
  }
  
}

//***********************************************************************************
//***********************************************************************************
void CSN_Pin(BYTE state,unsigned char W_RF)                                // Set/reset CSN pin
{
     switch(W_RF)
  {case 0 : 
    if(state)
    nRF24L01_CSN_1;
  else
    nRF24L01_CSN_0;   break;
    case 1 : 
    if(state)
    nRF24L01_CSN1_1;
  else
    nRF24L01_CSN1_0;   break;
    case 2 : 
    if(state)
    nRF24L01_CSN2_1;
  else
    nRF24L01_CSN2_0;   break;
    case 3 : 
    if(state)
    nRF24L01_CSN3_1;
  else
    nRF24L01_CSN3_0;   break;
    
  default:  break;
  }
}
//***********************************************************************************
//***********************************************************************************
void SCK_Pin(BYTE state,unsigned char W_RF)                                // Set/reset SCK pin
{
  switch(W_RF)
  {case 0 :  
  if(state)
    nRF24L01_SCK_1;
  else
    nRF24L01_SCK_0; break;
    
   case 1 :  
  if(state)
    nRF24L01_SCK1_1;
  else
    nRF24L01_SCK1_0; break;  
    
   case 2 :  
  if(state)
    nRF24L01_SCK2_1;
  else
    nRF24L01_SCK2_0; break;
      
   case 3 :  
  if(state)
    nRF24L01_SCK3_1;
  else
    nRF24L01_SCK3_0; break ;     
      
     default:  break;
  
  }
}
//***********************************************************************************
//***********************************************************************************
void MOSI_Pin(BYTE state,unsigned char W_RF)                               // Set/reset MOSI pin
{
  switch(W_RF)
  {case 0 :
  if(state)
    nRF24L01_MOSI_1;
  else
    nRF24L01_MOSI_0;  break;
    
    case 1 :
  if(state)
    nRF24L01_MOSI1_1;
  else
    nRF24L01_MOSI1_0;  break;
    
    case 2 :
  if(state)
    nRF24L01_MOSI2_1;
  else
    nRF24L01_MOSI2_0;  break;
    
    case 3 :
  if(state)
    nRF24L01_MOSI3_1;
  else
    nRF24L01_MOSI3_0;  break;
      
  default:  break;  
  }
}
//***********************************************************************************
//***********************************************************************************

BYTE MISO_Pin(unsigned char W_RF)                                     // Read MISO pin
{
   
 if(0 == W_RF)
  return nRF24L01_MISO_IN;
 if(1 == W_RF)
  return nRF24L01_MISO1_IN;  
 if(2 == W_RF)
  return nRF24L01_MISO2_IN; 
 if(3 == W_RF)
  return nRF24L01_MISO3_IN; 
  
  return 0;
}
//**************************************************
//Function: SPI_RW();

//escription:
 // Writes one byte to nRF24L01, and return the byte read
 // from nRF24L01 during write, according to SPI protocol
//**************************************************/
uchar SPI_RW(uchar byte,unsigned char W_RF)
{
	uchar bit_ctr;
   	for(bit_ctr=0;bit_ctr<8;bit_ctr++)   // output 8-bit
   	{
   		MOSI_Pin(byte&0x80,W_RF);         // output 'byte', MSB to MOSI
   		byte = (byte << 1);           // shift next bit into MSB..
   		SCK_Pin(1,W_RF);// Set SCK high..
   		byte|=MISO_Pin(W_RF);       		  // capture current MISO bit
   		SCK_Pin(0,W_RF);            		  // ..then set SCK low again
   	}
    return(byte);           		  // return read byte
}
/**************************************************/

//**************************************************
//Function: SPI_RW_Reg();

//Description:
 // Writes value 'value' to register 'reg'
//**************************************************/
uchar SPI_RW_Reg(BYTE reg, BYTE value,unsigned char W_RF)
{
        uchar status;
  	CSN_Pin(0,W_RF);                   // CSN low, init SPI transaction
  	status = SPI_RW(reg,W_RF);      // select register
  	SPI_RW(value,W_RF);             // ..and write value to it..
  	CSN_Pin(1,W_RF);                   // CSN high again

  	return(status);            // return nRF24L01 status byte
        
}
/**************************************************/

/**************************************************
Function: SPI_Read();

Description:
  Read one byte from nRF24L01 register, 'reg'
**************************************************/
BYTE SPI_Read(BYTE reg,unsigned char W_RF)
{
	BYTE reg_val;

  	CSN_Pin(0,W_RF);                // CSN low, initialize SPI communication...
  	SPI_RW(reg,W_RF);            // Select register to read from..
  	reg_val = SPI_RW(0,W_RF);    // ..then read registervalue
  	CSN_Pin(1,W_RF);                // CSN high, terminate SPI communication

  	return(reg_val);        // return register value
}
/**************************************************/

/**************************************************
Function: SPI_Read_Buf();

Description:
  Reads 'bytes' #of bytes from register 'reg'
  Typically used to read RX payload, Rx/Tx address
**************************************************/
uchar SPI_Read_Buf(BYTE reg, BYTE *pBuf, BYTE bytes,unsigned char W_RF)
{
	uchar status,byte_ctr;

  	CSN_Pin(0,W_RF);                    		// Set CSN low, init SPI tranaction
  	status = SPI_RW(reg,W_RF);       		// Select register to write to and read status byte

  	for(byte_ctr=0;byte_ctr<bytes;byte_ctr++)
    	pBuf[byte_ctr] = SPI_RW(0,W_RF);    // Perform SPI_RW to read byte from nRF24L01

  	CSN_Pin(1,W_RF);                           // Set CSN high again

  	return(status);                    // return nRF24L01 status byte
}
/**************************************************/

/**************************************************
Function: SPI_Write_Buf();

Description:
  Writes contents of buffer '*pBuf' to nRF24L01
  Typically used to write TX payload, Rx/Tx address
**************************************************/
uchar SPI_Write_Buf(BYTE reg, BYTE const *pBuf, BYTE bytes,unsigned char W_RF)
{
	uchar status,byte_ctr;

  	CSN_Pin(0,W_RF);                  // Set CSN low, init SPI tranaction
  	status = SPI_RW(reg,W_RF);    // Select register to write to and read status byte
  	for(byte_ctr=0; byte_ctr<bytes; byte_ctr++) // then write all byte in buffer(*pBuf)
    	SPI_RW(*pBuf++,W_RF);
  	CSN_Pin(1,W_RF);                 // Set CSN high again
  	
       
        return(status);          // return nRF24L01 status byte 
}
/**************************************************/

//***********************************************************************************
/**************************************************
Function: RX_Mode();

Description:
  This function initializes one nRF24L01 device to
  RX Mode, set RX address, writes RX payload width,
  select RF channel, datarate & LNA HCURR.
  After init, CE is toggled high, which means that
  this device is now ready to receive a datapacket.
**************************************************/
void RX_Mode(unsigned char W_RF)
{
      switch(W_RF){
      case 0:
        CE_Pin(0,W_RF);
	SPI_Write_Buf(RF_WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH,W_RF);    	
	SPI_Write_Buf(RF_WRITE_REG + RX_ADDR_P0, TX_ADDRESS, RX_ADR_WIDTH,W_RF); 
	SPI_RW_Reg(RF_WRITE_REG + EN_AA, ENAA_P0,W_RF); 
	SPI_RW_Reg(RF_WRITE_REG + SETUP_RETR,0x1a,W_RF); 
	SPI_RW_Reg(RF_WRITE_REG + EN_RXADDR, 0x01,W_RF);  //使能通道0的接收地址 
	SPI_RW_Reg(RF_WRITE_REG + RF_CH, 50,W_RF);        //设置RF通道为2.400GHz  频率=2.4+0GHz
	SPI_RW_Reg(RF_WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH,W_RF); 
	SPI_RW_Reg(RF_WRITE_REG + RF_SETUP, 0x0F,W_RF);   //设置TX发射参数,0db增益,2Mbps,低噪声增益开启
	SPI_RW_Reg(RF_WRITE_REG + CONFIG, 0x0f,W_RF);   
        CE_Pin(1,W_RF);   break;
      case 1:
        CE_Pin(0,W_RF);
	SPI_Write_Buf(RF_WRITE_REG + TX_ADDR, TX_ADDRESS1, TX_ADR_WIDTH,W_RF);    	
	SPI_Write_Buf(RF_WRITE_REG + RX_ADDR_P0, TX_ADDRESS1, RX_ADR_WIDTH,W_RF); 
	SPI_RW_Reg(RF_WRITE_REG + EN_AA, ENAA_P0,W_RF); 
	SPI_RW_Reg(RF_WRITE_REG + SETUP_RETR,0x1a,W_RF); 
	SPI_RW_Reg(RF_WRITE_REG + EN_RXADDR, 0x01,W_RF);  //使能通道0的接收地址 
	SPI_RW_Reg(RF_WRITE_REG + RF_CH, 10,W_RF);        //设置RF通道为2.400GHz  频率=2.4+0GHz
	SPI_RW_Reg(RF_WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH,W_RF); 
	SPI_RW_Reg(RF_WRITE_REG + RF_SETUP, 0x0F,W_RF);   //设置TX发射参数,0db增益,2Mbps,低噪声增益开启
	SPI_RW_Reg(RF_WRITE_REG + CONFIG, 0x0f,W_RF);   
        CE_Pin(1,W_RF);   break;     
      case 2:
        CE_Pin(0,W_RF);
	SPI_Write_Buf(RF_WRITE_REG + TX_ADDR, TX_ADDRESS2, TX_ADR_WIDTH,W_RF);    	
	SPI_Write_Buf(RF_WRITE_REG + RX_ADDR_P0, TX_ADDRESS2, RX_ADR_WIDTH,W_RF); 
	SPI_RW_Reg(RF_WRITE_REG + EN_AA, ENAA_P0,W_RF); 
	SPI_RW_Reg(RF_WRITE_REG + SETUP_RETR,0x1a,W_RF); 
	SPI_RW_Reg(RF_WRITE_REG + EN_RXADDR, 0x01,W_RF);  //使能通道0的接收地址 
	SPI_RW_Reg(RF_WRITE_REG + RF_CH, 10,W_RF);        //设置RF通道为2.400GHz  频率=2.4+0GHz
	SPI_RW_Reg(RF_WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH,W_RF); 
	SPI_RW_Reg(RF_WRITE_REG + RF_SETUP, 0x0F,W_RF);   //设置TX发射参数,0db增益,2Mbps,低噪声增益开启
	SPI_RW_Reg(RF_WRITE_REG + CONFIG, 0x0f,W_RF);   
        CE_Pin(1,W_RF);   break;     
      case 3:
        CE_Pin(0,W_RF);
	SPI_Write_Buf(RF_WRITE_REG + TX_ADDR, TX_ADDRESS3, TX_ADR_WIDTH,W_RF);    	
	SPI_Write_Buf(RF_WRITE_REG + RX_ADDR_P0, TX_ADDRESS3, RX_ADR_WIDTH,W_RF); 
	SPI_RW_Reg(RF_WRITE_REG + EN_AA, ENAA_P0,W_RF); 
	SPI_RW_Reg(RF_WRITE_REG + SETUP_RETR,0x1a,W_RF); 
	SPI_RW_Reg(RF_WRITE_REG + EN_RXADDR, 0x01,W_RF);  //使能通道0的接收地址 
	SPI_RW_Reg(RF_WRITE_REG + RF_CH, 10,W_RF);        //设置RF通道为2.400GHz  频率=2.4+0GHz
	SPI_RW_Reg(RF_WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH,W_RF); 
	SPI_RW_Reg(RF_WRITE_REG + RF_SETUP, 0x0F,W_RF);   //设置TX发射参数,0db增益,2Mbps,低噪声增益开启
	SPI_RW_Reg(RF_WRITE_REG + CONFIG, 0x0f,W_RF);   
        CE_Pin(1,W_RF);   break;     
     
      } 
}
/**************************************************/

/**************************************************
Function: TX_Mode();

Description:
  This function initializes one nRF24L01 device to
  TX mode, set TX address, set RX address for auto.ack,
  fill TX payload, select RF channel, datarate & TX pwr.
  PWR_UP is set, CRC(2 bytes) is enabled, & PRIM:TX.

  ToDo: One high pulse(>10us) on CE will now send this
  packet and expext an acknowledgment from the RX device.
**************************************************/
void TX_Mode(unsigned char W_RF)
{
	CE_Pin(0, W_RF);
	
  	SPI_Write_Buf(RF_WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH, W_RF);    	
	SPI_Write_Buf(RF_WRITE_REG + RX_ADDR_P0, TX_ADDRESS, RX_ADR_WIDTH, W_RF); 
	SPI_RW_Reg(RF_WRITE_REG + EN_AA, ENAA_P0, W_RF); 
      
        //SPI_RW_Reg(WRITE_REG + EN_AA, 0x00); 
	//SPI_RW_Reg(WRITE_REG + SETUP_RETR,0X00);//0x1a;0X13
	
        SPI_RW_Reg(RF_WRITE_REG + SETUP_RETR,0x0a, W_RF);       
        
	SPI_RW_Reg(RF_WRITE_REG + EN_RXADDR, 0x01, W_RF);  
	SPI_RW_Reg(RF_WRITE_REG + RF_CH, 50, W_RF);        
	SPI_RW_Reg(RF_WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH, W_RF); 
	SPI_RW_Reg(RF_WRITE_REG + RF_SETUP, 0x0F, W_RF);   //设置TX发射参数,0db增益,2Mbps,低噪声增益开启 
	SPI_RW_Reg(RF_WRITE_REG + CONFIG, 0x0e, W_RF);   
	CE_Pin(1, W_RF);

}
void CLearinterr(unsigned char W_RF)
{
CE_Pin(0, W_RF);
  	SPI_RW_Reg(RF_WRITE_REG+STATUS, 0X30, W_RF);
      
CE_Pin(1, W_RF);
}

/**************************************************/

void nRF24L01_Init(unsigned char Mode,unsigned char W_RF)
{
	TestBuf = SPI_Read(RX_ADDR_P4, W_RF);//return is 0xC5,if connection is ok

	if(Mode == nRF_TX_Mode)
		TX_Mode(W_RF);
	else RX_Mode(W_RF);
	delay_1ms(2);//24L01 掉电模式到待机模式需要1.5ms
}


void nRF24L01_Send(unsigned char W_RF)
{
		SPI_Write_Buf(WR_TX_PLOAD, tx_buf, TX_PLOAD_WIDTH,W_RF); // Writes data to TX payload

}



void nRF24L01_Revceive(unsigned char W_RF)
{

  
  switch(W_RF){
  case  0:	
  


       // if(ed2<ed1){ed2=ed1;}
        
               TestBuf = SPI_Read(STATUS, W_RF);
		SPI_RW_Reg(RF_WRITE_REG+STATUS,TestBuf, W_RF);
		if(TestBuf&RX_OK)
		{
			SPI_Read_Buf(RD_RX_PLOAD, NRF_rx_buf, RX_PLOAD_WIDTH, W_RF);
			SPI_RW_Reg(FLUSH_RX, 0xff, W_RF);
                      //  if(NRF_rx_buf[0]==0x1f)
//                         { TimeFalg.get_pp_time1=__HAL_TIM_GET_COUNTER(&htim4);//获取定时器的计数时间
//                           TimeFalg.get_pp_temp++;
//                           if(TimeFalg.get_pp_temp>=3)
//                           {TimeFalg.get_pp_temp=0;
//                            TimeFalg.get_pp_falg=1;                            
//                           } 
//                         }			
		}
              break;
              
              
  case  1:	
               TestBuf = SPI_Read(STATUS, W_RF);
		SPI_RW_Reg(RF_WRITE_REG+STATUS,TestBuf, W_RF);
		if(TestBuf&RX_OK)
		{     
			SPI_Read_Buf(RD_RX_PLOAD, NRF_rx_buf1, RX_PLOAD_WIDTH, W_RF);
			SPI_RW_Reg(FLUSH_RX, 0xff, W_RF);
                    //    if(NRF_rx_buf1[0]==0x1f)
//                         { TimeFalg.get_pp_time2=__HAL_TIM_GET_COUNTER(&htim4);//获取定时器的计数时间
//                           
//                           TimeFalg.get_pp_temp++;
//                           if(TimeFalg.get_pp_temp>=3)
//                           {TimeFalg.get_pp_temp=0;
//                            TimeFalg.get_pp_falg=1;
//                            }
//                           }
                         
                         			
		}
              break;
              
              
              
  case  2:	
               TestBuf = SPI_Read(STATUS, W_RF);
		SPI_RW_Reg(RF_WRITE_REG+STATUS,TestBuf, W_RF);
		if(TestBuf&RX_OK)
		{     
			SPI_Read_Buf(RD_RX_PLOAD, NRF_rx_buf2, RX_PLOAD_WIDTH, W_RF);
			SPI_RW_Reg(FLUSH_RX, 0xff, W_RF);
                 //       if(NRF_rx_buf2[0]==0x1f)
//                         { TimeFalg.get_pp_time3=__HAL_TIM_GET_COUNTER(&htim4);//获取定时器的计数时间
//                           TimeFalg.get_pp_temp++;
//                           if(TimeFalg.get_pp_temp>=3)
//                           {TimeFalg.get_pp_temp=0;
//                            TimeFalg.get_pp_falg=1;
//                          }                
//                          }			
		}
              break;


		
  }

}
/*
 * 函数名：NRF_Check
 * 描述  ：主要用于NRF与MCU是否正常连接
 * 输入  ：无	
 * 输出  ：SUCCESS/ERROR 连接正常/连接失败
 * 调用  ：外部调用
 */
uchar NRF_Check(unsigned char W_RF)
{
	uchar buf[5]={0xFF,0xFF,0xFF,0xFF,0xFF};
	uchar buf1[5];
	uchar i; 
        
CE_Pin(0, W_RF);	 
	/*写入5个字节的地址.  */  
	SPI_Write_Buf(RF_WRITE_REG+TX_ADDR,buf,5, W_RF);

	/*读出写入的地址 */
	SPI_Read_Buf(TX_ADDR,buf1,5, W_RF); 
	 
CE_Pin(1, W_RF);	/*比较*/               
	for(i=0;i<5;i++)
	{       
		if(buf1[i]!=0xFF)
		break;            
	} 
	       
	if(i==5)
		return 1 ;        //MCU与NRF成功连接 
	else
		return 0 ;        //MCU与NRF不正常连接
              
}
/*
void nRF24L01_IO_set(void)
{
      P4DIR |= BIT4;         //ce
      //P4OUT = BIT4;         //ce
      P4DIR |= BIT5;         //csn
      P5DIR |= BIT3;         //sck
      P5DIR |= BIT2;         //mosi out
      P5DIR &=~BIT1;         //MISO IN 
      P1DIR &=~BIT4;         //IRQ
      
      SCK_Pin(0);
      CE_Pin(0);
}
*/
void CLEAR_AD_Data(unsigned char W_RF)                  
{  	CSN_Pin(0,W_RF);

    SPI_RW(FLUSH_TX, W_RF);
    SPI_RW(RD_RX_PLOAD, W_RF);
  	CSN_Pin(1, W_RF);
}



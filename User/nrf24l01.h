#ifndef NRF24L01_H
#define NRF24L01_H


/*****
#define nRF24L01_CE_1        	P4OUT |=  BIT4              //CE = 1
#define nRF24L01_CE_0        	P4OUT &=~ BIT4            //CE = 0
#define nRF24L01_CSN_1       	P4OUT |=  BIT5             //CSN = 1
#define nRF24L01_CSN_0       	P4OUT &=~ BIT5            //CSN = 0
#define nRF24L01_SCK_1       	P5OUT |=  BIT3              //SCK = 1
#define nRF24L01_SCK_0       	P5OUT &=~ BIT3            //SCK = 0
#define nRF24L01_MOSI_1  	P5OUT |=  BIT2         //MOSI = 1
#define nRF24L01_MOSI_0  	P5OUT &=~ BIT2       //MOSI = 0
#define nRF24L01_MISO_IN     	((P5IN>>1)  & 0x01)    //读入MISO,P5.1//((P5IN >> 1) & 0x04) 
*********************************************/
/**************************************************/

//0

//=======================NRF24L01_CE端口=========================================
#define  nRF24L01_CE_0        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET)        
#define  nRF24L01_CE_1        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET)      
//=============================RF24L01_CSN端口==================================
#define  nRF24L01_CSN_0       HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET)          
#define  nRF24L01_CSN_1       HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET)     
//=============================RF24L01_SCK端口======================================
#define  nRF24L01_SCK_0       HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET)     
#define  nRF24L01_SCK_1       HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET)    
//=============================RF24L01_MISO端口=========================================
#define nRF24L01_MISO_IN     	HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6)
//============================= RF24L01_MOSI端口================================
#define  nRF24L01_MOSI_0      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET)
#define  nRF24L01_MOSI_1      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET)
//==========================IRQ状态============================================
#define nRF24L01_IRQ_IN      	HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_7)
//==========================电源控制端口============================================


//1


//=======================NRF24L01_CE端口=========================================
#define  nRF24L01_CE1_0        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET)        
#define  nRF24L01_CE1_1        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET)      
//=============================RF24L01_CSN端口==================================
#define  nRF24L01_CSN1_0       HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET)          
#define  nRF24L01_CSN1_1       HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET)     
//=============================RF24L01_SCK端口======================================
#define  nRF24L01_SCK1_0       HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET)     
#define  nRF24L01_SCK1_1       HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET)    
//=============================RF24L01_MISO端口=========================================
#define nRF24L01_MISO1_IN     	HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6)
//============================= RF24L01_MOSI端口================================
#define  nRF24L01_MOSI1_0      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET)
#define  nRF24L01_MOSI1_1      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET)
//==========================IRQ状态============================================
#define nRF24L01_IRQ1_IN      	HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_7)
//==========================电源控制端口============================================


//2


//=======================NRF24L01_CE端口=========================================
#define  nRF24L01_CE2_0        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET)        
#define  nRF24L01_CE2_1        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET)      
//=============================RF24L01_CSN端口==================================
#define  nRF24L01_CSN2_0       HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET)          
#define  nRF24L01_CSN2_1       HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET)     
//=============================RF24L01_SCK端口======================================
#define  nRF24L01_SCK2_0       HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET)     
#define  nRF24L01_SCK2_1       HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET)    
//=============================RF24L01_MISO端口=========================================
#define nRF24L01_MISO2_IN     	HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6)
//============================= RF24L01_MOSI端口================================
#define  nRF24L01_MOSI2_0      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET)
#define  nRF24L01_MOSI2_1      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET)
//==========================IRQ状态============================================
#define nRF24L01_IRQ2_IN      	HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_7)
//==========================电源控制端口============================================

//3


//=======================NRF24L01_CE端口=========================================
#define  nRF24L01_CE3_0        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET)        
#define  nRF24L01_CE3_1        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET)      
//=============================RF24L01_CSN端口==================================
#define  nRF24L01_CSN3_0       HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET)          
#define  nRF24L01_CSN3_1       HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET)     
//=============================RF24L01_SCK端口======================================
#define  nRF24L01_SCK3_0       HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET)     
#define  nRF24L01_SCK3_1       HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET)    
//=============================RF24L01_MISO端口=========================================
#define nRF24L01_MISO3_IN     	HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6)
//============================= RF24L01_MOSI端口================================
#define  nRF24L01_MOSI3_0      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET)
#define  nRF24L01_MOSI3_1      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET)
//==========================IRQ状态============================================
#define nRF24L01_IRQ3_IN      	HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_7)
//==========================电源控制端口============================================



//typedef struct{
//uint8_t  get_pp_temp;
//uint8_t  get_pp_falg;
//uint32_t get_pp_time1;
//uint32_t get_pp_time2;
//uint32_t get_pp_time3;
//}timfalg;




//void Init_NRF24L01_GPIO(unsigned char W_RF);
void CLEAR_AD_Data(unsigned char W_RF);
void CLearinterr(unsigned char W_RF);





#endif






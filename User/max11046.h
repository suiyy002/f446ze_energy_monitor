#ifndef  __MAX11046_H_
#define  __MAX11046_H_

#include "stm32f4xx_hal.h"
#include "main.h"

#define  AD_EOC_INUPT     (AD_EOC_GPIO_Port->IDR & AD_EOC_Pin ? 1 : 0)
#define  AD_DOUT0_INUPT   (AD_DB0_GPIO_Port->IDR & AD_DB0_Pin ? 1 : 0)
#define  AD_DOUT1_INUPT   (AD_DB1_GPIO_Port->IDR & AD_DB1_Pin ? 1 : 0)
#define  AD_DOUT2_INUPT   (AD_DB2_GPIO_Port->IDR & AD_DB2_Pin ? 1 : 0)
#define  AD_DOUT3_INUPT   (AD_DB3_GPIO_Port->IDR & AD_DB3_Pin ? 1 : 0)
#define  AD_DOUT4_INUPT   (AD_DB4_GPIO_Port->IDR & AD_DB4_Pin ? 1 : 0)
#define  AD_DOUT5_INUPT   (AD_DB5_GPIO_Port->IDR & AD_DB5_Pin ? 1 : 0)
#define  AD_DOUT6_INUPT   (AD_DB6_GPIO_Port->IDR & AD_DB6_Pin ? 1 : 0)
#define  AD_DOUT7_INUPT   (AD_DB7_GPIO_Port->IDR & AD_DB7_Pin ? 1 : 0)
#define  AD_DOUT8_INUPT   (AD_DB8_GPIO_Port->IDR & AD_DB8_Pin ? 1 : 0)
#define  AD_DOUT9_INUPT   (AD_DB9_GPIO_Port->IDR & AD_DB9_Pin ? 1 : 0)
#define  AD_DOUT10_INUPT  (AD_DB10_GPIO_Port->IDR & AD_DB10_Pin ? 1 : 0)
#define  AD_DOUT11_INUPT  (AD_DB11_GPIO_Port->IDR & AD_DB11_Pin ? 1 : 0)
#define  AD_DOUT12_INUPT  (AD_DB12_GPIO_Port->IDR & AD_DB12_Pin ? 1 : 0)
#define  AD_DOUT13_INUPT  (AD_DB13_GPIO_Port->IDR & AD_DB13_Pin ? 1 : 0)
#define  AD_DOUT14_INUPT  (AD_DB14_GPIO_Port->IDR & AD_DB14_Pin ? 1 : 0)
#define  AD_DOUT15_INUPT  (AD_DB15_GPIO_Port->IDR & AD_DB15_Pin ? 1 : 0)

#define  AD_CR0_OUTPUT_0  do{AD_DB0_GPIO_Port->BSRR = (uint32_t)AD_DB0_Pin << 16U;}while(0)
#define  AD_CR0_OUTPUT_1  do{AD_DB0_GPIO_Port->BSRR = AD_DB0_Pin;}while(0)
#define  AD_CR1_OUTPUT_0  do{AD_DB1_GPIO_Port->BSRR = (uint32_t)AD_DB1_Pin << 16U;}while(0)
#define  AD_CR1_OUTPUT_1  do{AD_DB1_GPIO_Port->BSRR = AD_DB1_Pin;}while(0)
#define  AD_CR2_OUTPUT_0  do{AD_DB2_GPIO_Port->BSRR = (uint32_t)AD_DB2_Pin << 16U;}while(0)
#define  AD_CR2_OUTPUT_1  do{AD_DB2_GPIO_Port->BSRR = AD_DB2_Pin;}while(0)
#define  AD_CR3_OUTPUT_0  do{AD_DB3_GPIO_Port->BSRR = (uint32_t)AD_DB3_Pin << 16U;}while(0)
#define  AD_CR3_OUTPUT_1  do{AD_DB3_GPIO_Port->BSRR = AD_DB3_Pin;}while(0)

#define  AD_CONVST_OUTPUT_0    do{AD_CONVST_GPIO_Port->BSRR = (uint32_t)AD_CONVST_Pin << 16U;}while(0)
#define  AD_CONVST_OUTPUT_1    do{AD_CONVST_GPIO_Port->BSRR = AD_CONVST_Pin;}while(0)
#define  AD_SHDN_OUTPUT_0      do{AD_SHDN_GPIO_Port->BSRR = (uint32_t)AD_SHDN_Pin << 16U;}while(0)
#define  AD_SHDN_OUTPUT_1      do{AD_SHDN_GPIO_Port->BSRR = AD_SHDN_Pin;}while(0)
#define  AD_WR_OUTPUT_0        do{AD_WR_GPIO_Port->BSRR = (uint32_t)AD_WR_Pin << 16U;}while(0)
#define  AD_WR_OUTPUT_1        do{AD_WR_GPIO_Port->BSRR = AD_WR_Pin;}while(0)
#define  AD_CS_OUTPUT_0        do{AD_CS_GPIO_Port->BSRR = (uint32_t)AD_CS_Pin << 16U;}while(0)
#define  AD_CS_OUTPUT_1        do{AD_CS_GPIO_Port->BSRR = AD_CS_Pin;}while(0)
#define  AD_RD_OUTPUT_0        do{AD_RD_GPIO_Port->BSRR = (uint32_t)AD_RD_Pin << 16U;}while(0)
#define  AD_RD_OUTPUT_1        do{AD_RD_GPIO_Port->BSRR = AD_RD_Pin;}while(0)



extern void Configuration_AD_register(void);
extern void AD_IO_INIT(void);
extern void getADdata(void);

#endif

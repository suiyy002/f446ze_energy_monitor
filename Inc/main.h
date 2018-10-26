/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2018 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H__
#define __MAIN_H__

/* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define RF_CE_Pin GPIO_PIN_0
#define RF_CE_GPIO_Port GPIOA
#define RF_CSN_Pin GPIO_PIN_1
#define RF_CSN_GPIO_Port GPIOA
#define RF_SCK_Pin GPIO_PIN_4
#define RF_SCK_GPIO_Port GPIOA
#define RF_MOSI_Pin GPIO_PIN_5
#define RF_MOSI_GPIO_Port GPIOA
#define RF_MISO_Pin GPIO_PIN_6
#define RF_MISO_GPIO_Port GPIOA
#define RF_IRQ_Pin GPIO_PIN_7
#define RF_IRQ_GPIO_Port GPIOA
#define LED2_Pin GPIO_PIN_12
#define LED2_GPIO_Port GPIOE
#define LED1_Pin GPIO_PIN_13
#define LED1_GPIO_Port GPIOE
#define AD_SHDN_Pin GPIO_PIN_14
#define AD_SHDN_GPIO_Port GPIOE
#define AD_CONVST_Pin GPIO_PIN_15
#define AD_CONVST_GPIO_Port GPIOE
#define AD_EOC_Pin GPIO_PIN_10
#define AD_EOC_GPIO_Port GPIOB
#define AD_DB0_Pin GPIO_PIN_11
#define AD_DB0_GPIO_Port GPIOB
#define AD_DB1_Pin GPIO_PIN_12
#define AD_DB1_GPIO_Port GPIOB
#define AD_DB2_Pin GPIO_PIN_13
#define AD_DB2_GPIO_Port GPIOB
#define AD_DB3_Pin GPIO_PIN_14
#define AD_DB3_GPIO_Port GPIOB
#define AD_DB4_Pin GPIO_PIN_15
#define AD_DB4_GPIO_Port GPIOB
#define AD_DB5_Pin GPIO_PIN_8
#define AD_DB5_GPIO_Port GPIOD
#define AD_DB6_Pin GPIO_PIN_9
#define AD_DB6_GPIO_Port GPIOD
#define AD_DB7_Pin GPIO_PIN_10
#define AD_DB7_GPIO_Port GPIOD
#define AD_DB8_Pin GPIO_PIN_11
#define AD_DB8_GPIO_Port GPIOD
#define AD_DB9_Pin GPIO_PIN_12
#define AD_DB9_GPIO_Port GPIOD
#define AD_DB10_Pin GPIO_PIN_13
#define AD_DB10_GPIO_Port GPIOD
#define AD_DB11_Pin GPIO_PIN_14
#define AD_DB11_GPIO_Port GPIOD
#define AD_DB12_Pin GPIO_PIN_15
#define AD_DB12_GPIO_Port GPIOD
#define AD_DB13_Pin GPIO_PIN_2
#define AD_DB13_GPIO_Port GPIOG
#define AD_DB14_Pin GPIO_PIN_3
#define AD_DB14_GPIO_Port GPIOG
#define AD_DB15_Pin GPIO_PIN_4
#define AD_DB15_GPIO_Port GPIOG
#define AD_RD_Pin GPIO_PIN_5
#define AD_RD_GPIO_Port GPIOG
#define AD_CS_Pin GPIO_PIN_6
#define AD_CS_GPIO_Port GPIOG
#define AD_WR_Pin GPIO_PIN_7
#define AD_WR_GPIO_Port GPIOG
#define GPS_INT_Pin GPIO_PIN_6
#define GPS_INT_GPIO_Port GPIOC
#define W25Q64_DO_Pin GPIO_PIN_0
#define W25Q64_DO_GPIO_Port GPIOD
#define W25Q64_CS_Pin GPIO_PIN_1
#define W25Q64_CS_GPIO_Port GPIOD
#define W25Q64_DI_Pin GPIO_PIN_2
#define W25Q64_DI_GPIO_Port GPIOD
#define W25Q64_CLK_Pin GPIO_PIN_3
#define W25Q64_CLK_GPIO_Port GPIOD
#define W25Q64_HOLD_Pin GPIO_PIN_4
#define W25Q64_HOLD_GPIO_Port GPIOD

/* ########################## Assert Selection ############################## */
/**
  * @brief Uncomment the line below to expanse the "assert_param" macro in the 
  *        HAL drivers code
  */
/* #define USE_FULL_ASSERT    1U */

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
 extern "C" {
#endif
void _Error_Handler(char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)
#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

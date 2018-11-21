
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_hal.h"
#include "cmsis_os.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include "freq_cap.h"
#include "power_quality.h"
#include "max11046.h"
#include "util.h"
#include "flash_internal.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
extern uint8_t AD_save_flag;
extern uint16_t AD_Data_A_0[128];  //C
extern uint16_t AD_Data_A_1[128];  //C
extern uint16_t AD_Data_B_0[128];  //B
extern uint16_t AD_Data_B_1[128];  //B
extern uint16_t AD_Data_C_0[128];  //A
extern uint16_t AD_Data_C_1[128];  //A
// #define __SORT_TEST__
#ifdef __SORT_TEST__
  #include "alg_findextrema.h"
  #include "alg_sort.h"
  float test_arr[512] = {0.1, 0.2, 0.3, 0.74, 0.53, 0.12, 0.7, 0.8};
  float test_arr_max_min[2] = {0};
#endif
#ifdef __UTIL_TEST__  /* util.c test */
  volatile uint16_t tmp_ = 0x1234;
  volatile uint32_t tmp_32 = 0x12345678;
  volatile uint64_t tmp_64 = 0x123456789abcdeff;
  volatile void* ptmp_ = NULL;
  volatile uint32_t* ptmp_32 = NULL;
  volatile uint64_t* ptmp_64 = NULL;
  volatile uint16_t* ptmp_16 = NULL;
#endif  /* util.c test */
// #define __FLASH_TEST__
#ifdef __FLASH_TEST__  /* flash_internal.c test */
  volatile uint16_t err_flg = 0x00;
  extern uint16_t ad_wav[2][3][6400]; // 1 second, 75KB
#endif  /* flash_internal.c test */
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_FREERTOS_Init(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM3_Init();
  MX_USART2_UART_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
  AD_IO_INIT();
  Configuration_AD_register();
  HAL_Delay(10);

  #ifdef __SORT_TEST__
    bubble_sort_f32(test_arr, 512);
    __nop();
    findmax_min_f32(test_arr, 512, test_arr_max_min);
    __nop();
  #endif
  #ifdef __FLASH_TEST__ /* flash_internal.c test */
    flash_erase(7); /* erase sector 7 */
    ad_wav[0][0][0] = 0x01;
    ad_wav[0][1][0] = 0x01;
    ad_wav[0][2][0] = 0x01;
    err_flg = flash_write((uint16_t*)ad_wav[0], 7, 19200);
  #endif /* flash_internal.c test */


  /* USER CODE END 2 */

  /* Call init function for freertos objects (in freertos.c) */
  MX_FREERTOS_Init();

  /* Start scheduler */
  osKernelStart();
  
  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

    #if __UTIL_TEST__ /* util.c test */
      ptmp_ = endian_exchange(tmp_, 2);
      tmp_ = *(uint16_t*)ptmp_;
      ptmp_32 = endian_exchange(tmp_32, 4);
      tmp_32 = *(uint32_t*)ptmp_32;
      ptmp_64 = endian_exchange(tmp_64, 8);
      tmp_64 = *(uint64_t*)ptmp_64;
    __nop();
    #endif /* util.c test */
    
  }
  /* USER CODE END 3 */

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 15;
  RCC_OscInitStruct.PLL.PLLN = 216;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Activate the Over-Drive mode 
    */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 15, 0);
}

/* USER CODE BEGIN 4 */
/*
  PC9 --> channel4 --> A
  PC8 --> channel3 --> B
  PC7 --> channel2 --> C
*/
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
  if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
  {
    if(pfreq->freq_bgn_flg[0] == 0)
    {
      pfreq->cap_last[0] = HAL_TIM_ReadCapturedValue(&htim3, TIM_CHANNEL_4);
      pfreq->freq_bgn_flg[0] = 1;
      if(pfreq->phase_bgn_flg[0] == 0)
      {
        pfreq->phase[0] = pfreq->cap_last[0];
        pfreq->phase_bgn_flg[0] = 1;
      }
    }
    else
    {
      pfreq->cap_cur[0] = HAL_TIM_ReadCapturedValue(&htim3, TIM_CHANNEL_4);
      
      pfreq->freq[0] =  TIM_FREQ / (/*TIM_PERIOD * pfreq->period[0] + */(pfreq->cap_last[0] >= pfreq->cap_cur[0] ? 
      TIM_PERIOD - (pfreq->cap_last[0] - pfreq->cap_cur[0]) : 
      pfreq->cap_cur[0] - pfreq->cap_last[0]));
      #if 0
      if(pfreq->freq[0] - (float)40.0 < (float)1.0)
      {
        __nop();
      }
      #endif
      pfreq->freq_bgn_flg[0] = 0;
      pfreq->cap_last[0] = pfreq->cap_cur[0];
      pfreq->freq_ready[0] = 1;
    }
  }
  else if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
  {
    if(pfreq->freq_bgn_flg[1] == 0)
    {
      pfreq->cap_last[1] = HAL_TIM_ReadCapturedValue(&htim3, TIM_CHANNEL_3);
      pfreq->freq_bgn_flg[1] = 1;
      if(pfreq->phase_bgn_flg[1] == 0)
      {
        pfreq->phase[1] = pfreq->cap_last[1];
        pfreq->phase_bgn_flg[1] = 1;
      }
    }
    else
    {
      pfreq->cap_cur[1] = HAL_TIM_ReadCapturedValue(&htim3, TIM_CHANNEL_3);
      
      pfreq->freq[1] = TIM_FREQ / (/*TIM_PERIOD * pfreq->period[1] + */(pfreq->cap_last[1] >= pfreq->cap_cur[1] ? 
      TIM_PERIOD - (pfreq->cap_last[1] - pfreq->cap_cur[1]) : 
      pfreq->cap_cur[1] - pfreq->cap_last[1]));
      
      pfreq->freq_bgn_flg[1] = 0;
      pfreq->cap_last[1] = pfreq->cap_cur[1];
      pfreq->freq_ready[1] = 1;
    }
  }
  else if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
  {
    if(pfreq->freq_bgn_flg[2] == 0)
    {
      pfreq->cap_last[2] = HAL_TIM_ReadCapturedValue(&htim3, TIM_CHANNEL_2);
      pfreq->freq_bgn_flg[2] = 1;
      if(pfreq->phase_bgn_flg[0] == 1 && pfreq->phase_bgn_flg[1] == 1)
      {        
        pfreq->phase[2] = pfreq->cap_last[2];
        #if 1
          /*
            假设测量的时候接线顺序和时间随机，很有可能造成问题，导致测量不准确（定时器多出几个周期什么的）
            对周期取模去除周期
          */
          pfreq->phase_us[0] = (pfreq->phase[1] >= pfreq->phase[0] ? 
          (pfreq->phase[1] - pfreq->phase[0]) : 
          ((pfreq->period[0] <= 1 ? 1 : pfreq->period[0]-1) * TIM_PERIOD) - pfreq->phase[0] + pfreq->phase[1]) % (uint32_t)(TIM_FREQ / pfreq->freq[0]);
          pfreq->phase_us[1] = (pfreq->phase[2] >= pfreq->phase[1] ? 
          (pfreq->phase[2] - pfreq->phase[1]) : 
          ((pfreq->period[1] <= 1 ? 1 : pfreq->period[1]-1) * TIM_PERIOD) - pfreq->phase[1] + pfreq->phase[2]) % (uint32_t)(TIM_FREQ / pfreq->freq[1]);
          pfreq->phase_us[2] = pfreq->phase_us[0] + pfreq->phase_us[1];
          #if 0
          if(pfreq->phase[0] < pfreq->phase[1]) pfreq->phase_us[0] = pfreq->phase[1] - pfreq->phase[0];
          else pfreq->phase_us[0] = TIM_PERIOD + pfreq->phase[1] - pfreq->phase[0];
          if(pfreq->phase[1] < pfreq->phase[2]) pfreq->phase_us[1] = pfreq->phase[2] - pfreq->phase[1];
          else pfreq->phase_us[0] = TIM_PERIOD + pfreq->phase[2] - pfreq->phase[1];
          pfreq->phase_us[2] = pfreq->phase_us[0] + pfreq->phase_us[1];
          #endif
          for(uint8_t i=0; i<3; i++)
          {
              pfreq->period[i] = 0;

              // pfreq->phase_ang[i] = ((uint32_t)((pfreq->phase_us[i] / (TIM_FREQ / pfreq->freq[i])) * 3600000) % 3600000) / 10000.0;
              pfreq->phase_ang[i] = (pfreq->phase_us[i] / (TIM_FREQ / pfreq->freq[i])) * 360.0;
          }
          pfreq->phase_ready = 1;
          for(uint8_t i=0; i<3; i++)
          {
            pfreq->phase_bgn_flg[i] = 0;
          }
        #endif
  //        for(uint8_t i=0; i<3; i++)
  //        {
  //          pfreq->phase_bgn_flg[i] = 2;
  //        }
      }
    }
    else
    {
      pfreq->cap_cur[2] = HAL_TIM_ReadCapturedValue(&htim3, TIM_CHANNEL_2);
      
      pfreq->freq[2] = TIM_FREQ / (/*TIM_PERIOD * pfreq->period[2] + */(pfreq->cap_last[2] >= pfreq->cap_cur[2] ? 
      TIM_PERIOD - (pfreq->cap_last[2] - pfreq->cap_cur[2]) : 
      pfreq->cap_cur[2] - pfreq->cap_last[2]));
      
      pfreq->freq_bgn_flg[2] = 0;
      pfreq->cap_last[2] = pfreq->cap_cur[2];
      pfreq->freq_ready[2] = 1;
    }
  }
  else;
}
/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM14 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */
  if(htim->Instance == TIM4)
  {
    getADdata();
    if (0x01 == AD_save_flag)
        Voltage_RMS_Calc(AD_Data_A_0, AD_Data_B_0, AD_Data_C_0);
    if (0x00 == AD_save_flag)
        Voltage_RMS_Calc(AD_Data_A_1, AD_Data_B_1, AD_Data_C_1);
  }
  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM14) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */
  #if 1
  else if(htim->Instance == TIM3)
  {
    if(pfreq->phase_bgn_flg[0] == 1)
    {
      pfreq->period[0]++;
    }
    else if(pfreq->phase_bgn_flg[1]  == 1)
    {
      pfreq->period[1]++;
    }
    
    else if(pfreq->phase_bgn_flg[2] == 1)
    {
      pfreq->period[2]++;
    }
    #endif
    else;
  }
  else;
  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

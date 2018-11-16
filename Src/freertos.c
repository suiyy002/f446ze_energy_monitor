/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

/* USER CODE BEGIN Includes */     
#include "main.h"
#include "stm32f4xx_hal.h"
#include "freq_cap.h"
#include "tim.h"
#include "power_quality.h"
#include "nrf24l01_reg.h"
#include "nrf24l01.h"
#include "delay.h"
// #include "w25q64.h"
#include "protocol.h"
/* USER CODE END Includes */

/* Variables -----------------------------------------------------------------*/
osThreadId defaultTaskHandle;

/* USER CODE BEGIN Variables */
#define dbg_w25q64
#define dbg_nrf

osThreadId nrfTaskHandle;

union _dgb_flg
{
  uint8_t flg_total;
  struct _flg_t
  {
    uint8_t nrf_send : 1;
    uint8_t nrf_rev : 1;
    uint8_t nrf_err : 1;
    uint8_t w25q64_err : 1;
    uint8_t nrf_send1 : 1;
    uint8_t nrf_send2 : 1;
    uint8_t nrf_send3 : 1;
    uint8_t nrf_send4 : 1;
  }flg;

}dgb_flg;
extern float rms_tab[];
/* USER CODE END Variables */

/* Function prototypes -------------------------------------------------------*/
void StartDefaultTask(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* USER CODE BEGIN FunctionPrototypes */
void StartNrfTask(void const * arg);
static void nrf_send(uint8_t *txbuf, uint8_t size);

/* USER CODE END FunctionPrototypes */

/* Hook prototypes */

/* Init FreeRTOS */

void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
       
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
//  osThreadDef(nrfTask, StartNrfTask, osPriorityLow, 0, 128);
//  nrfTaskHandle = osThreadCreate(osThread(nrfTask), NULL);
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */
}

/* StartDefaultTask function */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  
  // start capture
  HAL_TIM_Base_Start_IT(&htim3);
  HAL_TIM_Base_Start_IT(&htim4);
  HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_4);
  HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_3);
  HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_2);
  /* Infinite loop */
  for(;;)
  {
    data_process();
    osDelay(50);
    // 2 avoid conflict with default value 0
    #if 0
      if
      (1
      && pfreq->phase_bgn_flg[0] == 2
      && pfreq->phase_bgn_flg[1] == 2
      && pfreq->phase_bgn_flg[2] == 2
      )
      {
        pfreq->phase_us[0] = (pfreq->phase[1] >= pfreq->phase[0]
          ? (pfreq->phase[1] - pfreq->phase[0] + pfreq->period[0] * TIM_PERIOD)
          : (TIM_PERIOD - pfreq->phase[0] + pfreq->phase[1]) 
          + (pfreq->period[0] <= 1 ? 0 : pfreq->period[0]-1) * TIM_PERIOD);
        pfreq->phase_us[1] = (pfreq->phase[2] >= pfreq->phase[1]
          ? (pfreq->phase[2] - pfreq->phase[1] + pfreq->period[1] * TIM_PERIOD)
          : (TIM_PERIOD - pfreq->phase[1] + pfreq->phase[2]) 
          + (pfreq->period[1] <= 1 ? 0 : pfreq->period[1]-1) * TIM_PERIOD);
        pfreq->phase_us[2] = pfreq->phase_us[0] + pfreq->phase_us[1];
        for(uint8_t i=0; i<3; i++)
        {
          if(i==2)
            pfreq->phase_ang[i] =/* 2 * PHASE_OFFSET_ANG + */
            (pfreq->phase_us[i] / (TARGET_PERIOD * (TIM_FREQ / 1000000.0))) * 360.0;
          else
            pfreq->phase_ang[i] =/* PHASE_OFFSET_ANG + */
            (pfreq->phase_us[i] / (TARGET_PERIOD * (TIM_FREQ / 1000000.0))) * 360.0;
        }
        pfreq->phase_ready = 1;
        for(uint8_t i=0; i<3; i++)
        {
          pfreq->phase_bgn_flg[i] = 0;
        }
      }
    #endif
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Application */

/* nrf wrapper */
static void nrf_send(uint8_t *txbuf, uint8_t size)
{
  static uint8_t NRF_tx_buf[32] = {0};
  TX_Mode(0);
  delay_ms(1);
  if(size > 31)
    size = 31;
  NRF_tx_buf[0] = size;
  for(uint8_t i = 0; i < size; i++)
    NRF_tx_buf[i+1] = txbuf[i];
  SPI_Write_Buf(WR_TX_PLOAD, NRF_tx_buf, 32, 0);
  delay_ms(1);
  CLEAR_AD_Data(0);
  RX_Mode(0);
  delay_ms(1);
}
uint8_t readbuf[4];
void StartNrfTask(void const * arg)
{

  // initialization of nrf24
  while(!NRF_Check(0))
  {
    dgb_flg.flg.nrf_err = 1;
  }
  dgb_flg.flg.nrf_err = 0;
  
  //thread loop
  for(;;)
  {
    // do something
    if(dgb_flg.flg.nrf_send)
    {
      dgb_flg.flg.nrf_send = 0;
      for(uint8_t i=0; i<3; i++)
      {
        pfreq->freq[i] = 1;
        pfreq->phase_ang[i] = 1;
//        rms_tab[i] = i;
      }
      nrf_send_shortframe_1();
    }
    nRF24L01_Revceive(0);
    osDelay(5);
  }
}
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

#include "stm32f4xx_hal.h"
#include "max11046.h"
#include "math.h"


void MAX11046_delay(uint8_t i) {
    for (uint8_t k = 0; k < i; k++) { ;; }

}

void CRX_INPUT_INIT(void) {
    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_InitStruct.Pin = AD_DB0_Pin | AD_DB1_Pin | AD_DB2_Pin
                          | AD_DB3_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(AD_DB0_GPIO_Port, &GPIO_InitStruct);
}

void CRX_OUTPUT_INIT(void) {
    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_InitStruct.Pin = AD_DB0_Pin | AD_DB1_Pin | AD_DB2_Pin
                          | AD_DB3_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(AD_DB0_GPIO_Port, &GPIO_InitStruct);
}


uint16_t get_DB_IO_DATA(void) {
    uint16_t DB_DATA = 0;

    if (AD_DOUT0_INUPT) DB_DATA |= 0x0001;
    if (AD_DOUT1_INUPT) DB_DATA |= 0x0002;
    if (AD_DOUT2_INUPT) DB_DATA |= 0x0004;
    if (AD_DOUT3_INUPT) DB_DATA |= 0x0008;
    if (AD_DOUT4_INUPT) DB_DATA |= 0x0010;
    if (AD_DOUT5_INUPT) DB_DATA |= 0x0020;
    if (AD_DOUT6_INUPT) DB_DATA |= 0x0040;
    if (AD_DOUT7_INUPT) DB_DATA |= 0x0080;
    if (AD_DOUT8_INUPT) DB_DATA |= 0x0100;
    if (AD_DOUT9_INUPT) DB_DATA |= 0x0200;
    if (AD_DOUT10_INUPT) DB_DATA |= 0x0400;
    if (AD_DOUT11_INUPT) DB_DATA |= 0x0800;
    if (AD_DOUT12_INUPT) DB_DATA |= 0x1000;
    if (AD_DOUT13_INUPT) DB_DATA |= 0x2000;
    if (AD_DOUT14_INUPT) DB_DATA |= 0x4000;
    if (AD_DOUT15_INUPT) DB_DATA |= 0x8000;

    return DB_DATA;
}


uint16_t AD_00_data[1280] = {0};  //N
uint16_t AD_01_dataupper[1280] = {0};  //C
uint16_t AD_01_datalower[1280] = {0};  //C
uint16_t AD_02_dataupper[1280] = {0};  //B
uint16_t AD_02_datalower[1280] = {0};  //B
uint16_t AD_03_dataupper[1280] = {0};  //A
uint16_t AD_03_datalower[1280] = {0};  //A

uint16_t flicker_01_dataupper[128] = {0};  //C
uint16_t flicker_01_datalower[128] = {0};  //C
uint16_t flicker_02_dataupper[128] = {0};  //B
uint16_t flicker_02_datalower[128] = {0};  //B
uint16_t flicker_03_dataupper[128] = {0};  //A
uint16_t flicker_03_datalower[128] = {0};  //A

uint8_t AD_save_flag = 0, AD_save_flag1 = 0;


uint16_t AD_TEMP_data = 0;
uint8_t flicker_TEMP = 0;
uint16_t flicker_data = 0;
uint8_t AD_TEMP_flag = 0, flicker_flag = 0;

void getADdata(void) {   // unsigned int i=0;

    uint16_t AD_data[8] = {0};

    AD_CS_OUTPUT_1;
    AD_RD_OUTPUT_1;
    AD_WR_OUTPUT_1;

    AD_CONVST_OUTPUT_0;
    // MAX11046_delay(10);
    AD_CONVST_OUTPUT_1;
    // MAX11046_delay(100);
    //  MAX11046_delay(10);
    //  MAX11046_delay(100);
    AD_CONVST_OUTPUT_0;
    //if(0 == AD_EOC_INUPT)
    //{
    AD_CS_OUTPUT_0;
    for (uint8_t i = 0; i < 8; i++) { //??????8?????
        AD_RD_OUTPUT_0;                 // MAX11046_delay(10);
        AD_data[i] = get_DB_IO_DATA();  //????????????????
        AD_RD_OUTPUT_1;                 //  MAX11046_delay(10);
    }

    // todo ??????????
//    if (AD_data[3] >= 0x8000) { AD_data[3] = AD_data[3] - 0x8000; }
//    else if (AD_data[3] < 0x8000) { AD_data[3] = 0x8000 - AD_data[3]; }

/******************??FFT????????***************************/
    // AD_00_data[AD_TEMP_data]=AD_data[0];
    if (0x00 == AD_save_flag) {
        AD_01_dataupper[AD_TEMP_data] = AD_data[1]; // ?????AD?CH1,CH2,CH3
        AD_02_dataupper[AD_TEMP_data] = AD_data[2]; // ???8??????????
        AD_03_dataupper[AD_TEMP_data] = AD_data[3];
    }
/******************??FFT????????***************************/
    if (0x01 == AD_save_flag) {
        AD_01_datalower[AD_TEMP_data] = AD_data[1];
        AD_02_datalower[AD_TEMP_data] = AD_data[2];
        AD_03_datalower[AD_TEMP_data] = AD_data[3];
    }

    AD_TEMP_data++;
    if (AD_TEMP_data >= 1280) {
        AD_TEMP_data = 0;
        AD_TEMP_flag = 0x01;                // 1280 points sampled
        AD_save_flag++;
        //  HAL_TIM_Base_Stop_IT(&htim4);
        if (AD_save_flag >= 2) { AD_save_flag = 0; }
    }
/******************************************************************/
    flicker_TEMP++;
    if (flicker_TEMP >= 50) { 
        flicker_TEMP = 0;
        if (0x00 == AD_save_flag1) {
            flicker_01_dataupper[flicker_data] = AD_data[1];
            flicker_02_dataupper[flicker_data] = AD_data[2];
            flicker_03_dataupper[flicker_data] = AD_data[3];
        }
        if (0x01 == AD_save_flag1) {
            flicker_01_datalower[flicker_data] = AD_data[1];
            flicker_02_datalower[flicker_data] = AD_data[2];
            flicker_03_datalower[flicker_data] = AD_data[3];
        }
        flicker_data++;
        if (flicker_data >= 128) { 
            flicker_data = 0;
            flicker_flag = 0x01;
            AD_save_flag1++;
            if (AD_save_flag1 >= 2) { AD_save_flag1 = 0; }
        }
    }

    AD_RD_OUTPUT_1;
    AD_CS_OUTPUT_1;
    AD_CONVST_OUTPUT_1;

}


void Configuration_AD_register(void) {
    CRX_OUTPUT_INIT();
    AD_SHDN_OUTPUT_0;    // turn on
    AD_WR_OUTPUT_0;      // enable writing
    AD_CS_OUTPUT_0;      // select device
    AD_RD_OUTPUT_1;      // disable reading

    AD_CR0_OUTPUT_1;    // sample immediately after last convertion, rising edge at CONVST start the convertion 
    AD_CR1_OUTPUT_0;    // must be 0
    AD_CR2_OUTPUT_1;    // complement binary code
    AD_CR2_OUTPUT_0;    // shifted binary code
    AD_CR3_OUTPUT_0;    // internal voltage ref

    MAX11046_delay(10);
    AD_WR_OUTPUT_1;     // high for saving the config
    CRX_INPUT_INIT();
}

void AD_IO_INIT(void) {
    AD_CS_OUTPUT_0;
    AD_WR_OUTPUT_1;
    AD_RD_OUTPUT_1;
    AD_SHDN_OUTPUT_0;    // pull high to shutdown 
    AD_CONVST_OUTPUT_1;  // not sampling

}





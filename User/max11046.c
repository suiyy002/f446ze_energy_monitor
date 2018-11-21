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

uint16_t get_DB_IO_DATA(void) 
{
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


uint16_t AD_Data_A_0[128] = {0};  //C
uint16_t AD_Data_A_1[128] = {0};  //C
uint16_t AD_Data_B_0[128] = {0};  //B
uint16_t AD_Data_B_1[128] = {0};  //B
uint16_t AD_Data_C_0[128] = {0};  //A
uint16_t AD_Data_C_1[128] = {0};  //A

uint16_t ad_wav[2][3][1280] = {0}; // 0.2s , 15KB
uint16_t ad_wav_cnt = 0;
uint8_t ad_wav_flg = 0;
uint8_t ad_wav_valid = 0;
uint8_t AD_save_flag = 0, AD_save_flag1 = 0;


uint16_t AD_dat_cnt = 0;
uint8_t AD_har_cnt = 0;
uint8_t AD_har_flg = 0;
uint16_t AD_data[8] = {0};

void getADdata(void) 
{
    AD_CS_OUTPUT_1;
    AD_RD_OUTPUT_1;
    AD_WR_OUTPUT_1;
    AD_CONVST_OUTPUT_0;
    AD_CONVST_OUTPUT_1;
    AD_CONVST_OUTPUT_0;
    AD_CS_OUTPUT_0;

    for (uint8_t i = 0; i < 8; i++) 
    {
        AD_RD_OUTPUT_0;
        AD_data[i] = get_DB_IO_DATA();
        AD_RD_OUTPUT_1;
    }
    // AD_00_data[AD_dat_cnt]=AD_data[0];
    if (0x00 == AD_save_flag) 
    {
        AD_Data_A_0[AD_dat_cnt] = AD_data[1];
        AD_Data_B_0[AD_dat_cnt] = AD_data[2];
        AD_Data_C_0[AD_dat_cnt] = AD_data[3];
    }
    if (0x01 == AD_save_flag) 
    {
        AD_Data_A_1[AD_dat_cnt] = AD_data[1];
        AD_Data_B_1[AD_dat_cnt] = AD_data[2];
        AD_Data_C_1[AD_dat_cnt] = AD_data[3];
    }
    AD_dat_cnt++;
    if (AD_dat_cnt >= 128) 
    {
        AD_dat_cnt = 0;
        AD_save_flag++;
        if (AD_save_flag >= 2) 
        {
            AD_save_flag = 0; 
        }
        AD_har_cnt++;
        if(AD_har_cnt >= 10)
        {
            AD_har_flg = 1;
        }
    }
    ad_wav_cnt++;
    if(ad_wav_cnt >= 1280)
    {
        ad_wav_cnt = 0;
        ad_wav_flg ^= 1;
        ad_wav_valid = 1;
    }
    for(uint8_t i = 0; i < 3; i++)
    {
        ad_wav[!ad_wav_flg][i][ad_wav_cnt] = 
            (uint16_t)((((ad_wav[!ad_wav_flg][i][ad_wav_cnt] > 32768.0) ? 
            (ad_wav[!ad_wav_flg][i][ad_wav_cnt] - 32768.0) : 0)
            * 5.0 / 32768.0) * 100);
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





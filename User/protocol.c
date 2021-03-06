// includes
#include "stm32f4xx_hal.h"
#include "nrf24l01_reg.h"
#include "nrf24l01.h"
#include "freq_cap.h"
#include "power_quality.h"
#include "delay.h"
// macros
#define SENSOR_ID 0x00

#define VOL_SHORTFRAME_1_BGN 0xb0
#define VOL_SHORTFRAME_1_END 0xe0
#define SHORTFRAME_1_LEN 27
#define VOL_SHORTFRAME_2_BGN 0xb1
#define VOL_SHORTFRAME_2_END 0xe1
#define SHORTFRAME_2_LEN 27

#define VOL_LONGFRAME_BGN_TRAN 0xb2
#define TRANSIENT_BGN_A 0x0a
#define TRANSIENT_BGN_B 0x0b
#define TRANSIENT_BGN_C 0x0c
#define VOL_LONGFRAME_END_TRAN 0xe2
#define LONGFRAME_TRAN_LEN 28

#define VOL_LONGFRAME_BGN_HAR 0xb3
#define HARMONIC_BGN_A 0x0a
#define HARMONIC_BGN_B 0x0b
#define HARMONIC_BGN_C 0x0c
#define VOL_LONGFRAME_END_HAR 0xe3
#define LONGFRAME_HAR_LEN 27
// data struct
union _vol_shortframe_1
{
    struct _vol_shortframe_1_t
    {
        uint8_t bgn; // 0xb0
        uint8_t id;
        uint16_t vol_fluctuation_A;
        uint16_t vol_fluctuation_B;
        uint16_t vol_fluctuation_C;
        uint16_t vol_rms_A;
        uint16_t vol_rms_B;
        uint16_t vol_rms_C;
        uint16_t vol_phase_BA;
        uint16_t vol_phase_CB;
        uint16_t vol_phase_CA;
        int16_t vol_rms_diff_BA;
        int16_t vol_rms_diff_CB;
        int16_t vol_rms_diff_CA;
        int8_t end; // 0xe0
    }t;
    uint8_t t_tab[SHORTFRAME_1_LEN]; // total bytes
};
union _vol_shortframe_2
{
    struct _vol_shortframe_2_t
    {
        uint8_t bgn; // 0xb0
        uint8_t id;
        int16_t vol_freq_deviation_A;
        int16_t vol_freq_deviation_B;
        int16_t vol_freq_deviation_C;
        uint16_t vol_flicker_A; // 短时
        uint16_t vol_flicker_B;
        uint16_t vol_flicker_C;
        uint16_t vol_flicker_l_A; // 长时
        uint16_t vol_flicker_l_B;
        uint16_t vol_flicker_l_C;
        int16_t vol_rms_deviation_A;
        int16_t vol_rms_deviation_B;
        int16_t vol_rms_deviation_C;
        uint8_t end;// 0xe0
    }t;
    uint8_t t_tab[SHORTFRAME_2_LEN]; // total bytes
};
union _vol_longframe_1
{
    struct _vol_longframe_1_t
    {
        uint8_t bgn; // 0xb2
        uint8_t id; 
        uint8_t channel; // 0x0a or 0x0b or 0x0c
        uint16_t wavlen;
        uint16_t cnt; // 计数变量
        uint16_t wav16[10];
        uint8_t end; // 0xe2
    }t;
    uint8_t t_tab[LONGFRAME_TRAN_LEN]; // total bytes
};
union _vol_longframe_2
{
    struct _vol_longframe_2_t
    {
        uint8_t bgn; // 0xb3
        uint8_t id; // 0x00
        uint8_t channel; // 0x0a or 0x0b or 0x0c
        // uint16_t wavlen;
        uint8_t cnt; // 50 max or 49 max
        uint16_t wav16[10];
        uint16_t vol_THDu; 
        uint8_t end; // 0xe3
    }t;
    uint8_t t_tab[LONGFRAME_HAR_LEN]; // total bytes
};

// private variables
union _vol_shortframe_1 vol_shortframe_1;
union _vol_shortframe_2 vol_shortframe_2;
union _vol_longframe_1 vol_longframe_1;
union _vol_longframe_2 vol_longframe_2;

// uint8_t nrftxbuf[32];
// float phase_diff_tab[3];


// extern variables
extern freq_cap_t pfreq;
extern double rms_avg[2][3];// 均值, Voltage_Deviation_Calc
extern uint8_t flg_rms_avg;
extern double Data_deviation[3];
extern double fluctuation[3];
extern double flicker_Pst[3];
extern double flicker_Plt[3];
extern float HRUh[3][50];
extern float THDu[3];

// function prototypes
static void nrf_send(uint8_t *txbuf, uint8_t size);
void nrf_send_shortframe_1
(void
    // union _vol_shortframe_1 *p, 
    // void (*send)(uint8_t *txbuf, uint8_t size)
);



// fuctions
/* wrapper for nrf send */
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

void nrf_send_shortframe_1(void)
{
    #define T vol_shortframe_1.t
    #define TAB vol_shortframe_1.t_tab
    T.bgn = VOL_SHORTFRAME_1_BGN;
    T.id = SENSOR_ID;
    T.vol_fluctuation_A = (uint16_t)(fluctuation[0] * 100);
    T.vol_fluctuation_B = (uint16_t)(fluctuation[1] * 100);
    T.vol_fluctuation_C = (uint16_t)(fluctuation[2] * 100);
    T.vol_rms_A = (uint16_t)(rms_avg[!flg_rms_avg][0] * 100);
    T.vol_rms_B = (uint16_t)(rms_avg[!flg_rms_avg][1] * 100);
    T.vol_rms_C = (uint16_t)(rms_avg[!flg_rms_avg][2] * 100);
    T.vol_phase_BA = (uint16_t)(pfreq->phase_ang[0] * 100);
    T.vol_phase_CB = (uint16_t)(pfreq->phase_ang[1] * 100);
    T.vol_phase_CA = (uint16_t)(pfreq->phase_ang[2] * 100);
    T.vol_rms_diff_BA = (int16_t)((T.vol_rms_B - T.vol_rms_A) * 100);
    T.vol_rms_diff_CB = (int16_t)((T.vol_rms_C - T.vol_rms_B) * 100);
    T.vol_rms_diff_CA = (int16_t)((T.vol_rms_C - T.vol_rms_A) * 100);
    T.end = VOL_SHORTFRAME_1_END;
    
    nrf_send(TAB, sizeof(TAB));
    #undef TAB
    #undef T
}

void nrf_send_shortframe_2(void)
{
    #define T vol_shortframe_2.t
    #define TAB vol_shortframe_2.t_tab
    T.bgn = VOL_SHORTFRAME_2_BGN;
    T.id = SENSOR_ID;
    T.vol_freq_deviation_A = (int16_t)((pfreq->freq[0] - VOL_STD_FREQ) * 100);
    T.vol_freq_deviation_B = (int16_t)((pfreq->freq[1] - VOL_STD_FREQ) * 100);
    T.vol_freq_deviation_C = (int16_t)((pfreq->freq[2] - VOL_STD_FREQ) * 100);
    T.vol_flicker_A = (uint16_t)(flicker_Pst[0] * 100);
    T.vol_flicker_B = (uint16_t)(flicker_Pst[1] * 100);
    T.vol_flicker_C = (uint16_t)(flicker_Pst[2] * 100);
    T.vol_flicker_l_A = (uint16_t)(flicker_Plt[0] * 100);
    T.vol_flicker_l_A = (uint16_t)(flicker_Plt[1] * 100);
    T.vol_flicker_l_A = (uint16_t)(flicker_Plt[2] * 100);
    T.vol_rms_deviation_A = (int16_t)(Data_deviation[0] * 100);
    T.vol_rms_deviation_B = (int16_t)(Data_deviation[1] * 100);
    T.vol_rms_deviation_C = (int16_t)(Data_deviation[2] * 100);
    T.end = VOL_SHORTFRAME_2_END;

    nrf_send(TAB, SHORTFRAME_2_LEN);
    #undef TAB
    #undef T
}

void nrf_send_longframe_1(uint16_t buf[], uint16_t wavlen, uint16_t chl)
{
    #define T vol_longframe_1.t
    #define TAB vol_longframe_1.t_tab
    #define cnt_inc(n) T.cnt += n
    #define _cnt T.cnt
    #define wav T.wav16
    uint16_t num_of_frame_full = wavlen / 10;
    uint16_t num_of_frame_notfull = wavlen % 10;

    T.bgn = VOL_LONGFRAME_BGN_HAR;
    T.id = SENSOR_ID;
    T.channel = chl;
    T.wavlen = wavlen;
    T.cnt = 0x00;
    T.end = VOL_LONGFRAME_END_HAR;
    
    for(; _cnt < num_of_frame_full; cnt_inc(10))
    {
        for(uint16_t i = 0; i < 10; i++)
        {
            wav[i] = buf[i];
        }
        nrf_send(TAB, sizeof(TAB));
        // delay_ms(50);
    }
    if(num_of_frame_notfull)
    {
        for(uint16_t i = 0; i < 10; i++)
        {
            wav[i] = 0;
        }
        for(uint16_t i = 0; i < num_of_frame_notfull; i++)
        {
            wav[i] = (uint16_t)((((buf[i + _cnt] > 32768.0) ? (buf[i + _cnt] - 32768.0) : 0) * 5.0 / 32768.0) * 100);
        }
        cnt_inc(1);
        nrf_send(TAB, sizeof(TAB));
        // delay_ms(50);
    }
    #undef wav
    #undef _cnt
    #undef cnt_inc
    #undef TAB
    #undef T
}

void nrf_send_longframe_2(uint16_t buf[], uint16_t chl)
{
    #define T vol_longframe_2.t
    #define TAB vol_longframe_2.t_tab
    #define cnt_inc(n) T.cnt += n
    #define _cnt T.cnt
    #define wav T.wav16
    // uint16_t num_of_frame_full = wavlen / 10;
    // uint16_t num_of_frame_notfull = wavlen % 10;

    T.bgn = VOL_LONGFRAME_BGN_TRAN;
    T.id = SENSOR_ID;
    T.channel = chl;
    T.cnt = 0x00;
    T.end = VOL_LONGFRAME_END_TRAN;
    T.vol_THDu = (uint16_t)(THDu[chl] * 10);
    for(; _cnt < 10; cnt_inc(10))
    {
        for(uint16_t i = 0; i < 10; i++)
        {
            wav[i] = (uint16_t)(HRUh[chl][i] * 10);
        }
        nrf_send(TAB, sizeof(TAB));
        // delay_ms(50);
    }

    #undef wav
    #undef _cnt
    #undef cnt_inc
    #undef TAB
    #undef T
}

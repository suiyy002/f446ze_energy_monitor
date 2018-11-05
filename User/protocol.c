// includes
#include "stm32f4xx_hal.h"
#include "nrf24l01_reg.h"
#include "nrf24l01.h"
#include "delay.h"
#include "freq_cap.h"
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
#define LONGFRAME_TRAN_LEN 27

#define VOL_LONGFRAME_BGN_HAR 0xb3
#define HARMONIC_BGN_A 0x0a
#define HARMONIC_BGN_B 0x0b
#define HARMONIC_BGN_C 0x0c
#define VOL_LONGFRAME_END_HAR 0xe3
#define LONGFRAME_HAR_LEN 27

// global variables
union _vol_shortframe_1
{
    struct _vol_shortframe_1_t
    {
        uint8_t bgn; // 0xb0
        uint8_t id;
        union
        {
            uint16_t val;
            struct
            {
                uint8_t hi;
                uint8_t lo; 
            }val_t;
        }vol_fluctuation_A;
        union
        {
            uint16_t val;
            struct
            {
                uint8_t hi;
                uint8_t lo; 
            }val_t;
        }vol_fluctuation_B;
        union
        {
            uint16_t val;
            struct
            {
                uint8_t hi;
                uint8_t lo; 
            }val_t;
        }vol_fluctuation_C;
        union
        {
            uint16_t val;
            struct
            {
                uint8_t hi;
                uint8_t lo; 
            }val_t;
        }vol_rms_A;
        union
        {
            uint16_t val;
            struct
            {
                uint8_t hi;
                uint8_t lo; 
            }val_t;
        }vol_rms_B;
        union
        {
            uint16_t val;
            struct
            {
                uint8_t hi;
                uint8_t lo; 
            }val_t;
        }vol_rms_C;
        union
        {
            uint16_t val;
            struct
            {
                uint8_t hi;
                uint8_t lo; 
            }val_t;
        }vol_phase_BA;
        union
        {
            uint16_t val;
            struct
            {
                uint8_t hi;
                uint8_t lo; 
            }val_t;
        }vol_phase_CB;
        union
        {
            uint16_t val;
            struct
            {
                uint8_t hi;
                uint8_t lo; 
            }val_t;
        }vol_phase_CA;
        uint8_t end;// 0xe0
    }t;
    uint8_t t_tab[SHORTFRAME_1_LEN]; // total bytes
}vol_shortframe_1;
struct _vol_shortframe_2
{
    uint8_t bgn; // 0xb1
    uint8_t id;
    union
    {
        uint16_t val;
        struct
        {
            uint8_t hi;
            uint8_t lo; 
        }val_t;
    }vol_freq_deviation_A;
    union
    {
        uint16_t val;
        struct
        {
            uint8_t hi;
            uint8_t lo; 
        }val_t;
    }vol_freq_deviation_B;
    union
    {
        uint16_t val;
        struct
        {
            uint8_t hi;
            uint8_t lo; 
        }val_t;
    }vol_freq_deviation_C;
    union
    {
        uint16_t val;
        struct
        {
            uint8_t hi;
            uint8_t lo; 
        }val_t;
    }vol_flicker_A;
    union
    {
        uint16_t val;
        struct
        {
            uint8_t hi;
            uint8_t lo; 
        }val_t;
    }vol_flicker_B;
    union
    {
        uint16_t val;
        struct
        {
            uint8_t hi;
            uint8_t lo; 
        }val_t;
    }vol_flicker_C;
    union
    {
        uint16_t val;
        struct
        {
            uint8_t hi;
            uint8_t lo; 
        }val_t;
    }vol_rms_deviation_A;
    union
    {
        uint16_t val;
        struct
        {
            uint8_t hi;
            uint8_t lo; 
        }val_t;
    }vol_rms_deviation_B;
    union
    {
        uint16_t val;
        struct
        {
            uint8_t hi;
            uint8_t lo; 
        }val_t;
    }vol_rms_deviation_C;
    uint8_t end; // 0xc1
}vol_shortframe_2;
struct _vol_longframe_1
{
    uint8_t bgn; // 0xb2
    uint8_t id; 
    uint8_t bgn_tran; // 0x0a or 0x0b or 0x0c
    uint8_t cnt; // 128 max or 127 max
    union
    {
        uint16_t wav16[10];
        uint8_t wav8[20];
    }wav;
    uint8_t end; // 0xe2

}vol_longframe_1;
struct _vol_longframe_2
{
    uint8_t bgn; // 0xb3
    uint8_t id; // 0x00
    uint8_t bgn_har; // 0x0a or 0x0b or 0x0c
    uint8_t cnt; // 50 max or 49 max
    union
    {
        uint16_t wav16[10];
        uint8_t wav8[20];
    }wav;
    union
    {
        uint16_t val;
        struct
        {
            uint8_t hi;
            uint8_t lo; 
        }val_t;
    }vol_HR; // harmonic ratio
    uint8_t end; // 0xe3
}vol_longframe_2;
// uint8_t nrftxbuf[32];
// float phase_diff_tab[3];

// extern variables
extern float rms_tab[];
extern freq_cap_t pfreq;


// function prototypes
static void nrf_send(uint8_t *txbuf, uint8_t size);
void nrf_send_shortframe_1
(
    union _vol_shortframe_1 *p, 
    void (*send)(uint8_t *txbuf, uint8_t size)
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

void nrf_send_shortframe_1
(
    union _vol_shortframe_1 *p, 
    void (*send)(uint8_t *txbuf, uint8_t size)
)
{
    p->t.bgn = VOL_SHORTFRAME_1_BGN;
    p->t.id = SENSOR_ID;
    p->t.vol_phase_BA.val = (uint16_t)(pfreq->phase_ang[0] * 100);
    p->t.vol_phase_CB.val = (uint16_t)(pfreq->phase_ang[1] * 100);
    p->t.vol_phase_CA.val = (uint16_t)(pfreq->phase_ang[2] * 100);
    p->t.vol_rms_A.val = (uint16_t)(rms_tab[0] * 100);
    p->t.vol_rms_B.val = (uint16_t)(rms_tab[1] * 100);
    p->t.vol_rms_C.val = (uint16_t)(rms_tab[2] * 100);
    p->t.end = VOL_SHORTFRAME_1_END;

    send(p->t_tab, SHORTFRAME_1_LEN);
}
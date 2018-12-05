#include "fft.h"
#include "main.h"
#include "stm32f4xx_hal.h"
#include "arm_math.h"
#include "nrf24l01.h"
#include "nrf24l01_reg.h"


#define FFT_LENGTH  128

float fft_inputbuf[FFT_LENGTH * 2];
float fft_harmonicA_output[FFT_LENGTH];
float fft_harmonicB_output[FFT_LENGTH];
float fft_harmonicC_output[FFT_LENGTH]; 

float fft_inputbuf2[1024];
float fft_flicker_out_A[512];
float fft_flicker_out_B[512];
float fft_flicker_out_C[512];
//arm_cfft_radix4_instance_f32 scfft;
arm_cfft_radix2_instance_f32 scfft2;

void fft_harmonic_calc(uint16_t *p, uint8_t id_)
{
//    uint16_t i = 0;
    arm_cfft_radix2_init_f32(&scfft2, FFT_LENGTH, 0, 1);
    for (uint16_t i = 0; i < FFT_LENGTH; i++)
    {
        /* 提前修正，最后直流量需要除2 */
        fft_inputbuf[2 * i + 1] = (float)((*(p + i)) / (FFT_LENGTH / 2));
        fft_inputbuf[2 * i] = 0.00000;
    }
    arm_cfft_radix2_f32(&scfft2, fft_inputbuf);
    float *p2 = NULL;
    switch(id_)
    {
        case 0x0a:
        {
            p2 = fft_harmonicA_output;
            break;
        }
        case 0x0b:
        {
            p2 = fft_harmonicB_output;
            break;
        }
        case 0x0c:
        {
            p2 = fft_harmonicC_output;
            break;
        }
        default:
        {
            p2 = fft_harmonicA_output;
        }
    }
    arm_cmplx_mag_f32(fft_inputbuf, p2, FFT_LENGTH);
    p2[0] /= 2; /* 修正 */
    
}

void fft_flicker_calc(float *p, uint8_t id_)
{
    arm_cfft_radix2_init_f32(&scfft2, 512, 0, 1);
    for(uint16_t i = 0; i < 512; i++)
    {
        // 提前除256做修正，之后直流分量除2修正得到最终幅值
        fft_inputbuf2[2 * i + 1] = (float)((*(p + i)) / 256.0); 
        fft_inputbuf2[2 * i] = 0.00;
    }
    arm_cfft_radix2_f32(&scfft2, fft_inputbuf2);
    float *p2 = NULL;
    switch(id_)
    {
        case 0x0a:
        {
            p2 = fft_flicker_out_A;
            break;
        }
        case 0x0b:
        {
            p2 = fft_flicker_out_B;
            break;
        }
        case 0x0c:
        {
            p2 = fft_flicker_out_C;
            break;
        }
        default:
        {
            p2 = fft_flicker_out_A;
        }
    }
    arm_cmplx_mag_f32(fft_inputbuf2, p2, 512);
    p2[0] /= 2; /* 修正 */
}












#ifndef __FFT_H_
#define __FFT_H_

#include "stm32f4xx_hal.h"


extern void fft_harmonic_calc(uint16_t *p,uint8_t id_);
extern void fft_flicker_calc(float *p, uint8_t id_);


#endif




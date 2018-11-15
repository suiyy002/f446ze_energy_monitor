// #ifndef __ANALYSIS_H__
// #define __ANALYSIS_H__
#include "stm32f4xx_hal.h"

#define VOL_STD_FREQ 50.00
/* 57/180*3.53 volt */
#define VOL_STD_RMS_A  1.11783333333333
#define VOL_STD_RMS_B  1.11783333333333
#define VOL_STD_RMS_C  1.11783333333333
/* 180/3.53 transformer ratio */
#define VOL_STD_FACTOR 50.9915014164305949
#define PI_ 3.141592654

#define A_line 0X0A
#define B_line 0X0B
#define C_line 0X0C

extern void data_process(void);
extern void Voltage_Harmonic_Calc(void);
extern void Voltage_Deviation_Calc(void);
extern void Voltage_sag_swell_interruption(void);
extern void Voltage_RMS_Calc(uint16_t ad1[128], uint16_t ad2[128], uint16_t ad3[128]);
// #endif


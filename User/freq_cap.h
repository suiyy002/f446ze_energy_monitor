#ifndef __FREQ_CAP_H__
#define __FREQ_CAP_H__

#include "stm32f4xx_hal.h"
#define T freq_cap_t
/* the ARR value + 1 of the timer for capture */
#define TIM_PERIOD (0xffff+1)
/* the frequency of the timer for capture */
#define TIM_FREQ 1000000.0
/* 20000us */
// #define TARGET_PERIOD 20000.0
#define PHASE_OFFSET_ANG 0
typedef struct T *T;
struct T
{
  uint32_t cap_cur[3]; // new cap data
  uint32_t cap_last[3]; // old cap data
  
  float freq[3];
//  float freq_filtered[3];
  uint32_t phase[3];
  uint32_t phase_us[3];
  float phase_ang[3];
  uint8_t freq_ready[3]; // capture complete
  uint8_t phase_ready;
  
  uint32_t period[3]; // period numbers during two rising edge
  
  uint8_t freq_bgn_flg[3];
  uint8_t phase_bgn_flg[3];

};

extern T pfreq;

#undef T
#endif

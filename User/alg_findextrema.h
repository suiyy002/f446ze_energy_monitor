#ifndef __ALG_FINDEXTREMA_H__
#define __ALG_FINDEXTREMA_H__
#include "stm32f4xx_hal.h"
extern void findExtrema(
    float *src, uint16_t src_len, uint16_t distance,
    uint16_t *idx_max_tab, uint16_t *max_quantity,
    uint16_t *idx_min_tab, uint16_t *min_quantity,
    uint8_t id);
extern void findmax_min_f64(double *pbuf, uint16_t size, double *max_min_tab);
extern void findmax_min_f32(float *pbuf, uint16_t size, float *max_min_tab);
#endif // !__ALG_FINDEXTREMA_H__

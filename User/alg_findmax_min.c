#include "stm32f4xx_hal.h"
// const float EPSINON32 = 0.0000001;
// const double EPSINON64 = 0.00000000001;

void findmax_min_f32(float *pbuf, uint16_t size, float *max_min_tab)
{
    float max = pbuf[0], min = pbuf[0];
    for(uint16_t i = 1; i < size; i++)
    {
        if(pbuf[i] > max)
        {
            max = pbuf[i];
        }
        else if(pbuf[i] < min)
        {
            min = pbuf[i];
        }
    }
    max_min_tab[0] = max;
    max_min_tab[1] = min;
}
void findmax_min_f64(double *pbuf, uint16_t size, double *max_min_tab)
{
    double max = pbuf[0], min = pbuf[0];
    for(uint16_t i = 1; i < size; i++)
    {
        if(pbuf[i] > max)
        {
            max = pbuf[i];
        }
        else if(pbuf[i] < min)
        {
            min = pbuf[i];
        }
    }
    max_min_tab[0] = max;
    max_min_tab[1] = min;
}
#include "stm32f4xx_hal.h"
 

void bubble_sort_f32(float *pbuf, uint16_t n)
{
    uint16_t i, j;
    float temp = 0;
    for (j = 0; j < n - 1; j++)
        for (i = 0; i < n - 1 - j; i++)
        {
            if(pbuf[i] < pbuf[i + 1])
            {
                temp = pbuf[i];
                pbuf[i] = pbuf[i + 1];
                pbuf[i + 1] = temp;
            }
        }
}

void bubble_sort_f64(double *pbuf, uint16_t n)
{
    uint16_t i, j;
    double temp = 0;
    for (j = 0; j < n - 1; j++)
        for (i = 0; i < n - 1 - j; i++)
        {
            if(pbuf[i] < pbuf[i + 1])
            {
                temp = pbuf[i];
                pbuf[i] = pbuf[i + 1];
                pbuf[i + 1] = temp;
            }
        }
}
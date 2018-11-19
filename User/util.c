#include "stm32f4xx_hal.h"

/* 正整数大小端转换，in-place */
/* integer -- 任意指针， bytes -- 包含字节数 */
void big_little_endian(void* integer, uint8_t bytes)
{
    switch(bytes)
    {
        case 1:
        {
            return;
        }
        case 2:
        {
            *(uint16_t *)integer = ((((*(uint16_t *)integer)>>0)&0xff)<<8) 
                                 + ((((*(uint16_t *)integer)>>8)&0xff)>>0);
            return;
        }
        case 4:
        {
            *(uint32_t *)integer = ((((*(uint32_t *)integer)>>0)&0xff)<<24) 
                                 + ((((*(uint32_t *)integer)>>8)&0xff)<<16) 
                                 + ((((*(uint32_t *)integer)>>16)&0xff)<<8) 
                                 + ((((*(uint32_t *)integer)>>24)&0xff)>>0);
            return;
        }
        case 8:
        {
            *(uint64_t *)integer = ((((*(uint64_t *)integer)>>0)&0xff)<<56) 
                                 + ((((*(uint64_t *)integer)>>8)&0xff)<<48) 
                                 + ((((*(uint64_t *)integer)>>16)&0xff)<<40) 
                                 + ((((*(uint64_t *)integer)>>24)&0xff)>>32)
                                 + ((((*(uint64_t *)integer)>>32)&0xff)>>24)
                                 + ((((*(uint64_t *)integer)>>40)&0xff)>>16)
                                 + ((((*(uint64_t *)integer)>>48)&0xff)>>8)
                                 + ((((*(uint64_t *)integer)>>56)&0xff)<<0) ;
            return;
        }
        default:
            return;
    }
}

/* bytes：1，2，4，8 */
/*
    tmp_32 = *(uint32_t*)endian_exchange(tmp_32, 4);
*/
void* endian_exchange(uint64_t x, uint8_t bytes)
{
    static uint64_t tmp = 0;
    tmp = 0;
    /* 先进行位的交换 */
    for(uint8_t i = 0; i < bytes; i++)
    {
        tmp |= ((x>>(i*8))&0xff)<<((bytes-1-i)*8);
    }
    return &tmp;
}

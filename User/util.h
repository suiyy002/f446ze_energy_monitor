#ifndef __UTIL_H__
#define __UTIL_H__
#include "stm32f4xx_hal.h"
void big_little_endian(void* integer, uint8_t bytes);
void* endian_exchange(uint64_t x, uint8_t bytes);
#endif

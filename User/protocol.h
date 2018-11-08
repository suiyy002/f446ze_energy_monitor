// interface file for nrf24 protocol
#if !defined(__PROTOCOL_H__)
#define __PROTOCOL_H__

// includes
#include "stm32f4xx_hal.h"


// extern prototype
extern void nrf_send_shortframe_1(void);
extern void nrf_send_shortframe_2(void);
extern void nrf_send_longframe_1(uint16_t buf[], uint16_t wavlen, uint16_t chl);
#endif

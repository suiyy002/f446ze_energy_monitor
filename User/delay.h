#ifndef __delay_h__
#define __delay_h__

// #include "stm32f4xx_hal.h"
#include "cmsis_os.h"
#define delay_ms(t) osDelay(t)
// #define delay_ms(t) HAL_Delay(t)
#endif // !__delay_h__


#ifndef _CPU_DELAY_H_
#define _CPU_DELAY_H_

#ifdef ESP32
    #include "Esp32Delay.h"
    #define CpuDelay_init       Esp32Delay_init
    #define CpuDelay_destroy    Esp32Delay_destroy
    #define CpuDelay_ms         Esp32Delay_ms
    #define CpuDelay_us         Esp32Delay_us
#else
    #error "board/hardware is not defined"
#endif

#endif /* _CPU_DELAY_H_ */


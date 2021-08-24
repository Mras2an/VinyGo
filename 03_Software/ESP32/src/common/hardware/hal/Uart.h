
#ifndef _UART_H_
#define _UART_H_

#ifdef ESP32
    #include "Esp32Uart.h"
    #define  Uart_init   Esp32Uart_init
    #define  Uart_write  Esp32Uart_write
    #define  Uart_read   Esp32Uart_read
#else
    #error "board/hardware is not defined"
#endif

#endif /* _UART_H_ */

#ifndef ESP32_SPI_H_
#define ESP32_SPI_H_

#ifdef ESP32
    #include "Esp32Spi.h"

    #define Spi_init      Esp32Spi_init

    #define Spi_readByte   Esp32Spi_readByte

    #define Spi_writeByte  Esp32Spi_writeByte

    #define Spi_powerDown  Esp32Spi_powerDown

#else
    #error "board/hardware is not defined"
#endif

#endif /* ESP32_SPI_H_ */


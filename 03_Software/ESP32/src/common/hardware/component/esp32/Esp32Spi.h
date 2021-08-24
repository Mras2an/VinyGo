
#ifndef _ESP32SPI_H_
#define _ESP32SPI_H_

#include "Utils.h"

typedef enum {
    SPI_RESULT_OK                   = TRUE,
    SPI_RESULT_FAILED               = FALSE,
} SpiOpResult;

SpiOpResult Esp32Spi_init(void);

unsigned char Esp32Spi_readByte(unsigned char bSampleAtPositiveEdgeArg);

SpiOpResult Esp32Spi_writeByte(unsigned char btDataArg);

SpiOpResult Esp32Spi_powerDown(void);


#endif /* _ESP32SPI_H_ */


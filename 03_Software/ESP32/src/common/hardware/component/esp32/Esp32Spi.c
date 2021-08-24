#include "Esp32Spi.h"
#include "driver/spi_master.h"
#include "soc/gpio_struct.h"
#include "driver/gpio.h"
#include "Bsp.h"
#include "Gpio.h"
#include "CpuDelay.h"
#include "Board.h"

spi_device_handle_t spi;

/*!************************************************************************************************
* \fn         SpiOpResult Esp32Spi_init(void)
* \return     SpiOpResult
***************************************************************************************************/
SpiOpResult Esp32Spi_init(void)
{
    Debug_info("\n");
    esp_err_t ret;
    spi_bus_config_t buscfg = {
        .miso_io_num = PIN_NUM_MISO,
        .mosi_io_num = PIN_NUM_MOSI,
        .sclk_io_num = PIN_NUM_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1
    };
    spi_device_interface_config_t devcfg = {
        .clock_speed_hz = 2000000,                 //Clock out at 2 MHz
        .mode = 0,                                 //SPI mode 0
        .queue_size = 1,                           //We want to be able to queue 7 transactions at a time
    };
    //NO DMA
    ret = spi_bus_initialize(HSPI_HOST, &buscfg, 0);
    assert(ret == ESP_OK);
    ret = spi_bus_add_device(HSPI_HOST, &devcfg, &spi);
    assert(ret == ESP_OK);
    return SPI_RESULT_OK;
}

/*!************************************************************************************************
* \fn         unsigned char Esp32Spi_readByte(unsigned char bSampleAtPositiveEdgeArg)
* \param  bSampleAtPositiveEdgeArg
* \return     unsigned char
***************************************************************************************************/
unsigned char Esp32Spi_readByte(unsigned char bSampleAtPositiveEdgeArg)
{
    esp_err_t ret;
    uint8_t tx_data[] = {0x00};
    uint8_t rx_data[6];
    spi_transaction_t trans = {
        .length = 1 * 8,
        .tx_buffer = tx_data,
        .rx_buffer = rx_data
    };
    ret = spi_device_transmit(spi, &trans);
    assert(ret == ESP_OK);
    Debug_info("-> 0x%02x,\n", rx_data[0]);
    return (rx_data[0]);
}

/*!************************************************************************************************
* \fn         SpiOpResult Esp32Spi_writeByte(unsigned char btDataArg)
* \param  btDataArg
* \return     SpiOpResult
***************************************************************************************************/
SpiOpResult Esp32Spi_writeByte(unsigned char btDataArg)
{
    esp_err_t ret;
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));                 //Zero out the transaction
    t.length = 1*8;                           //Len is in byte(s), transaction length is in bits.
    t.tx_buffer = &btDataArg;                 //Data
    t.user=(void*)0;                          //D/C needs to be set to 1
    ret = spi_device_transmit(spi, &t);       //Transmit!
    assert(ret == ESP_OK);                    //Should have had no issues.
    return SPI_RESULT_OK;
}

/*!************************************************************************************************
* \fn         SpiOpResult Esp32Spi_cleanup(void)
* \return     SpiOpResult
***************************************************************************************************/
SpiOpResult Esp32Spi_cleanup(void)
{
    Debug_info("\n");
    SpiOpResult ret = Esp32Spi_powerDown();
    return ret;
}

/*!************************************************************************************************
* \fn         SpiOpResult Esp32Spi_powerDown(void)
* \return     SpiOpResult
***************************************************************************************************/
SpiOpResult Esp32Spi_powerDown(void)
{
    Debug_info("\n");

    if(spi_bus_remove_device(spi) != ESP_OK)
        return SPI_RESULT_FAILED;

    return SPI_RESULT_OK;
}

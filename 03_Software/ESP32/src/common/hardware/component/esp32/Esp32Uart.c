#ifdef ESP32
#include "driver/uart.h"
#include "Bsp.h"
typedef struct sUart {
    int uartNum;
} sUart_t;

static sUart_t sUart;
#define this (&sUart)

#define BUF_SIZE (1024)

/*!*****************************************************************************
 * \fn         void Esp32Uart_init(const int uartNum)
 * \param[in]  const int
 * \return     void
 *******************************************************************************/
void Esp32Uart_init(const int uartNum, const int baud, int pinRx, int pinTx)
{
    this->uartNum = uartNum;
    uart_config_t uart_config = {
        .baud_rate = baud,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .rx_flow_ctrl_thresh = 122,
    };
    uart_param_config(this->uartNum, &uart_config);
    uart_set_pin(this->uartNum, pinTx, pinRx, 0, 0);
    uart_driver_install(this->uartNum, BUF_SIZE, 0, 0, NULL, 0);
}

/*!*****************************************************************************
 * \fn         void Esp32Uart_write(const char * data, int len)
 * \param[in]  const char *
 * \param[in]  int
 * \return     void
 *******************************************************************************/
void Esp32Uart_write(const int uartNum, int len, const char * data)
{
    uart_write_bytes(uartNum, (const char*) data, len);
}

/*!*****************************************************************************
 * \fn         int Esp32Uart_read(int len, unsigned char * data)
 * \param[in]  char *
 * \param[in]  int
 * \return     int
 *******************************************************************************/
int Esp32Uart_read(const int uartNum, int len, unsigned char * data)
{
    len = uart_read_bytes(uartNum, data, len, OsPortTimingPeriod);
    return len;
}

#endif

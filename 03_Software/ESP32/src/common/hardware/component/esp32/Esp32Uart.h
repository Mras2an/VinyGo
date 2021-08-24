
#ifndef ESP32_UART_H_
#define ESP32_UART_H_

void Esp32Uart_init(const int uartNum, const int baud, int pinRx, int pinTx);

void Esp32Uart_write(const int uartNum, int len, const char * data);

int Esp32Uart_read(const int uartNum, int len, unsigned char * data);

#endif /* ESP32_UART_H_ */

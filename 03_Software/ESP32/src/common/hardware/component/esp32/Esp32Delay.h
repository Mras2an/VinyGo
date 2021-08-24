#ifndef ESP32_DELAY_H_
#define ESP32_DELAY_H_

#include "Debug.h"

eVrError_t Esp32Delay_init(void);

eVrError_t Esp32Delay_destroy(void);

void  __attribute__((optimize(0))) Esp32Delay_ms(short delay);

void  __attribute__((optimize(0))) Esp32Delay_us(short delay);

#endif /* ESP32_DELAY_H_ */

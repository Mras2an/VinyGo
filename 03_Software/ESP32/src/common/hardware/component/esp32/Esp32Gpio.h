#ifndef _ESP32_GPIO_H_
#define _ESP32_GPIO_H_

#include "Gpio.h"

eVrError_t Esp32Gpio_configure(eGpioNum_t pin, eGpioMode_t mode, eGpioPull_t pull);
eVrError_t Esp32Gpio_set(eGpioNum_t pin, uint32_t value);
eVrError_t Esp32Gpio_get(eGpioNum_t pin, uint32_t *value);
eVrError_t Esp32Gpio_toggle(eGpioNum_t pin);

#endif // _GPIO_H_


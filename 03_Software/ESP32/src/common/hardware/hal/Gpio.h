#ifndef _GPIO_H_
#define _GPIO_H_

#include "Bsp.h"

typedef enum eGpioNum {
    VR_GPIO_NUM_0=0,
    VR_GPIO_NUM_1,
    VR_GPIO_NUM_2,
    VR_GPIO_NUM_3,
    VR_GPIO_NUM_4,
    VR_GPIO_NUM_5,
    VR_GPIO_NUM_6,
    VR_GPIO_NUM_7,
    VR_GPIO_NUM_8,
    VR_GPIO_NUM_9,
    VR_GPIO_NUM_10,
    VR_GPIO_NUM_11,
    VR_GPIO_NUM_12,
    VR_GPIO_NUM_13,
    VR_GPIO_NUM_14,
    VR_GPIO_NUM_15,
    VR_GPIO_NUM_16,
    VR_GPIO_NUM_17,
    VR_GPIO_NUM_18,
    VR_GPIO_NUM_19,
    VR_GPIO_NUM_20,
    VR_GPIO_NUM_21,
    VR_GPIO_NUM_22,
    VR_GPIO_NUM_23,
    VR_GPIO_NUM_24,
    VR_GPIO_NUM_25,
    VR_GPIO_NUM_26,
    VR_GPIO_NUM_27,
    VR_GPIO_NUM_28,
    VR_GPIO_NUM_29,
    VR_GPIO_NUM_30,
    VR_GPIO_NUM_31,
    VR_GPIO_NUM_32,
    VR_GPIO_NUM_33,
    VR_GPIO_NUM_34,
    VR_GPIO_NUM_35,
    VR_GPIO_NUM_36,
    VR_GPIO_NUM_37,
    VR_GPIO_NUM_38,
    VR_GPIO_NUM_39,
    VR_GPIO_NUM_MAX = 40,
} eGpioNum_t;


typedef enum eGpioMode {
    VR_GPIO_MODE_INPUT,
    VR_GPIO_MODE_OUTPUT,
    VR_GPIO_MODE_OUTPUT_OPEN_DRAIN,
    VR_GPIO_MODE_INPUT_OUTPUT,
    VR_GPIO_MODE_INPUT_OUTPUT_OPEN_DRAIN,
} eGpioMode_t;

typedef enum eGpioPull {
    VR_GPIO_PULL_NONE,
    VR_GPIO_PULL_UP,
    VR_GPIO_PULL_DOWN,
} eGpioPull_t;

typedef enum eGpioLevel {
    VR_LEVEL_LOW,
    VR_LEVEL_HIGH,
} eGpioLevel_t;


#ifdef ESP32
    #include "Esp32Gpio.h"
    #define Gpio_init       Esp32Gpio_init
    #define Gpio_destroy    Esp32Gpio_destroy
    #define Gpio_configure  Esp32Gpio_configure
    #define Gpio_set        Esp32Gpio_set
    #define Gpio_get        Esp32Gpio_get
    #define Gpio_toggle     Esp32Gpio_toggle
#else
    #error "board/hardware is not defined"
#endif

#endif // _GPIO_H_


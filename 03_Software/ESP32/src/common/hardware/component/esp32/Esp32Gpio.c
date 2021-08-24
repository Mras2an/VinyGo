#ifdef ESP32

#include "driver/gpio.h"
#include "Esp32Gpio.h"
#include "Debug.h"

/******************************************************************************
 * \fn : eVrError_t Esp32Gpio_init(void)
 * \param   : none
 * \return      : eVrError_t
 *******************************************************************************/
eVrError_t Esp32Gpio_init(void)
{
    return VR_NO_ERROR;
}

/******************************************************************************
 * \fn : eVrError_t Esp32Gpio_destroy(void)
 * \param   : none
 * \return      : eVrError_t
 *******************************************************************************/
eVrError_t Esp32Gpio_destroy(void)
{
    return VR_NO_ERROR;
}

/******************************************************************************
 * \fn : eVrError_t Esp32Gpio_configure(eGpioNum_t pin, eGpioMode_t mode, eGpioPull_t pull)
 * \param   : uint32_t pin
 * \param   : eGpioMode mode
 * \param   : eGpioPull_t pull
 * \return      : eVrError_t
 *******************************************************************************/
eVrError_t Esp32Gpio_configure(eGpioNum_t pin, eGpioMode_t mode, eGpioPull_t pull)
{
    if(pin >= VR_GPIO_NUM_MAX) {
        return VR_ERROR(VR_INVALID_PARAMETER);
    }

    gpio_mode_t m;

    switch(mode) {
        case VR_GPIO_MODE_INPUT:
            m = GPIO_MODE_INPUT;
            break;

        case VR_GPIO_MODE_OUTPUT:
            m=GPIO_MODE_OUTPUT;
            break;

        case VR_GPIO_MODE_OUTPUT_OPEN_DRAIN:
            m= GPIO_MODE_OUTPUT_OD;
            break;

        case VR_GPIO_MODE_INPUT_OUTPUT:
            m= GPIO_MODE_INPUT_OUTPUT;
            break;

        case VR_GPIO_MODE_INPUT_OUTPUT_OPEN_DRAIN:
            m= GPIO_MODE_INPUT_OUTPUT_OD;
            break;

        default:
            return VR_ERROR(VR_INVALID_PARAMETER);
    }

    gpio_pullup_t up;
    gpio_pulldown_t down;

    switch(pull) {
        case VR_GPIO_PULL_NONE:
            up = GPIO_PULLUP_DISABLE;
            down = GPIO_PULLDOWN_DISABLE;
            break;

        case VR_GPIO_PULL_UP:
            up = GPIO_PULLUP_ENABLE;
            down = GPIO_PULLDOWN_ENABLE;
            break;

        case VR_GPIO_PULL_DOWN:
            up = GPIO_PULLUP_DISABLE;
            down = GPIO_PULLDOWN_ENABLE;
            break;

        default:
            return VR_ERROR(VR_INVALID_PARAMETER);
    }

    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
    io_conf.mode = m;
    io_conf.pin_bit_mask = (((uint64_t) 1) << pin);
    io_conf.pull_down_en = down;
    io_conf.pull_up_en = up;

    if(gpio_config(&io_conf) != ESP_OK) {
        return VR_ERROR(VR_INVALID_PARAMETER);
    }

    return VR_NO_ERROR;
}

/******************************************************************************
 * \fn : eVrError_t Esp32Gpio_set(eGpioNum_t pin, uint32_t value)
 * \param   : eGpioNum_t, uint32_t
 * \return      : none
 *******************************************************************************/
eVrError_t Esp32Gpio_set(eGpioNum_t pin, uint32_t value)
{
    if(gpio_set_level(pin, value) != ESP_OK) {
        return VR_ERROR(VR_INVALID_PARAMETER);
    }

    return VR_NO_ERROR;
}

/******************************************************************************
 * \fn : eVrError_t Esp32Gpio_get(eGpioNum_t pin, uint32_t *value)
 * \param   : eGpioNum_t, *uint32_t
 * \return      : eVrError_t
 *******************************************************************************/
eVrError_t Esp32Gpio_get(eGpioNum_t pin, uint32_t *value)
{
    *value = gpio_get_level(pin);
    return VR_NO_ERROR;
}

/******************************************************************************
 * \fn : eVrError_t Esp32Gpio_toggle(eGpioNum_t pin)
 * \param   : eGpioNum_t pin
 * \return      : eVrError_t
 *******************************************************************************/
eVrError_t Esp32Gpio_toggle(eGpioNum_t pin)
{
    if(gpio_get_level(pin)) {
        gpio_set_level(pin, 0);
    } else {
        gpio_set_level(pin, 1);
    }

    return VR_NO_ERROR;
}

#endif // ESP32


#ifdef ESP32
#include "Bsp.h"

#define   ESP32_NOP()   asm("NOP");

/******************************************************************************
 * \fn : eVrError_t Esp32Delay_init(void)
 * \param   : none
 * \return      : eVrError_t
 *******************************************************************************/
eVrError_t Esp32Delay_init(void)
{
    return VR_NO_ERROR;
}

/******************************************************************************
 * \fn : eVrError_t Esp32Delay_destroy(void)
 * \param   : none
 * \return      : eVrError_t
 *******************************************************************************/
eVrError_t Esp32Delay_destroy(void)
{
    return VR_NO_ERROR;
}

/******************************************************************************
 * \fn : void  __attribute__((optimize(0))) Esp32Delay_ms(short delay)
 * \param   : none
 * \return      : void
 *******************************************************************************/
void __attribute__((optimize(0))) Esp32Delay_ms(short delay)
{
    vTaskDelay(delay / portTICK_RATE_MS);
}
/******************************************************************************
 * \fn : void  __attribute__((optimize(0))) Esp32Delay_us(short delay)
 * \param   : none
 * \return      : void
 *******************************************************************************/
void __attribute__((optimize(0))) Esp32Delay_us(short delay)
{
    while(delay > 0) {
        /* In order to ensure that the loop will be not optimized by compiler */
        ets_delay_us(1);
        delay--;
    }
}
#endif // ESP32

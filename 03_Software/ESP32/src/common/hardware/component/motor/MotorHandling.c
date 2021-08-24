#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_attr.h"

#include "driver/mcpwm.h"
#include "driver/gpio.h"
#include "soc/mcpwm_reg.h"
#include "soc/mcpwm_struct.h"
#include "MotorHandling.h"
#include "Debug.h"
#include "Board.h"
#include "Bsp.h"
#include "Utils.h"
#include "freertos/FreeRTOS.h"
#include "freertos/portmacro.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "Timer.h"

#include "driver/pcnt.h"
#include "esp_attr.h"
#include "esp_log.h"
#include "soc/gpio_sig_map.h"

typedef struct sHookMotorHandling {
    double freq;
    int currentFreq;
} sHookMotorHandling_t;
static sHookMotorHandling_t sHookMotorHandling;
#define this (&sHookMotorHandling)

/*!*****************************************************************************
 * \fn         void MotorHandling_init()
 * \param      none
 * \return     none
 *******************************************************************************/
void MotorHandling_init()
{
    Debug_info("initializing mcpwm gpio...\n");
    this->currentFreq = 0;
    VR_ERROR_CHECK(
        Gpio_configure(STEP_MOTOR_ENABLE, VR_GPIO_MODE_INPUT_OUTPUT, VR_GPIO_PULL_DOWN));
    VR_ERROR_CHECK(Gpio_set(STEP_MOTOR_ENABLE, VR_LEVEL_LOW));
    VR_ERROR_CHECK(
        Gpio_configure(STEP_MOTOR_DIR, VR_GPIO_MODE_INPUT_OUTPUT, VR_GPIO_PULL_NONE));
    VR_ERROR_CHECK(Gpio_set(STEP_MOTOR_DIR, VR_LEVEL_LOW));
    VR_ERROR_CHECK(
        Gpio_configure(CMD_MOTOR_EN1, VR_GPIO_MODE_INPUT_OUTPUT, VR_GPIO_PULL_UP));
    VR_ERROR_CHECK(Gpio_set(CMD_MOTOR_EN1, VR_LEVEL_LOW));
    VR_ERROR_CHECK(
        Gpio_configure(CMD_MOTOR_INA1, VR_GPIO_MODE_INPUT_OUTPUT, VR_GPIO_PULL_UP));
    VR_ERROR_CHECK(Gpio_set(CMD_MOTOR_INA1, VR_LEVEL_LOW));
    //    VR_ERROR_CHECK(
    //        Gpio_configure(STEP_MOTOR_STEP, VR_GPIO_MODE_INPUT_OUTPUT, VR_GPIO_PULL_NONE));
    //    VR_ERROR_CHECK(Gpio_set(STEP_MOTOR_STEP, VR_LEVEL_LOW));
    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, STEP_MOTOR_STEP);
    mcpwm_gpio_init(MCPWM_UNIT_1, MCPWM1A, CMD_SERVO);
}

/*!*****************************************************************************
 * \fn         static void MotorHandling_forward(mcpwm_unit_t mcpwm_num, mcpwm_timer_t timer_num , float duty_cycle)
 * \param      mcpwm_num
 * \param      timer_num
 * \param      duty_cycle
 * \return     none
 *******************************************************************************/
static void MotorHandling_forward(mcpwm_unit_t mcpwm_num, mcpwm_timer_t timer_num , float duty_cycle)
{
    mcpwm_set_duty(mcpwm_num, timer_num, MCPWM_OPR_A, duty_cycle);
    mcpwm_set_duty_type(mcpwm_num, timer_num, MCPWM_OPR_A, MCPWM_DUTY_MODE_0);
}

/*!*****************************************************************************
 * \fn         static void MotorHandling_backward(mcpwm_unit_t mcpwm_num, mcpwm_timer_t timer_num , float duty_cycle)
 * \param      mcpwm_num
 * \param      timer_num
 * \param      duty_cycle
 * \return     none
 *******************************************************************************/
static void MotorHandling_backward(mcpwm_unit_t mcpwm_num, mcpwm_timer_t timer_num , float duty_cycle)
{
    mcpwm_set_duty(mcpwm_num, timer_num, MCPWM_OPR_B, duty_cycle);
    mcpwm_set_duty_type(mcpwm_num, timer_num, MCPWM_OPR_B, MCPWM_DUTY_MODE_0);
}

/*!*****************************************************************************
 * \fn         void MotorHandling_stop()
 * \param      none
 * \return     none
 *******************************************************************************/
void MotorHandling_stop()
{
    mcpwm_set_signal_low(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A);
    //mcpwm_set_signal_low(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_B);
    //mcpwm_set_signal_low(MCPWM_UNIT_1, MCPWM_TIMER_1, MCPWM_OPR_A);
    //mcpwm_set_signal_low(MCPWM_UNIT_1, MCPWM_TIMER_1, MCPWM_OPR_B);
    VR_ERROR_CHECK(Gpio_set(STEP_MOTOR_DIR, VR_LEVEL_LOW));
}

/*!*****************************************************************************
 * \fn         void MotorHandling_setDirection(int direction)
 * \param      direction
 * \return     none
 *******************************************************************************/
void MotorHandling_setDirection(int direction)
{
    if(direction == 0) {
        VR_ERROR_CHECK(Gpio_set(STEP_MOTOR_DIR, VR_LEVEL_LOW));
    } else {
        VR_ERROR_CHECK(Gpio_set(STEP_MOTOR_DIR, VR_LEVEL_HIGH));
    }
}

/*!*****************************************************************************
 * \fn         int MotorHandling_getDirection()
 * \param      none
 * \return     direction
 *******************************************************************************/
int MotorHandling_getDirection()
{
    uint32_t direction[2] = {0x00, 0x00};
    Gpio_get(STEP_MOTOR_DIR, &direction[0]);

    if(direction[0]) {
        return 1;
    }

    return 0;
}

/*!*****************************************************************************
 * \fn         void MotorHandling_start(int freq, int direction)
 * \param      none
 * \return     none
 *******************************************************************************/
void MotorHandling_start(int freq, int direction)
{
    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, STEP_MOTOR_STEP);
    MotorHandling_setDirection(direction);
    /*/!\ freq min 60Hz*/
    Debug_info("Configuring motor (freq:%d Hz)...\n", freq);
    mcpwm_config_t pwm_configX;
    pwm_configX.frequency = (uint32_t)freq;
    pwm_configX.cmpr_a = 0;
    pwm_configX.cmpr_b = 0;
    pwm_configX.counter_mode = MCPWM_UP_COUNTER;
    pwm_configX.duty_mode = MCPWM_DUTY_MODE_0;
    mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_configX);
    MotorHandling_forward(MCPWM_UNIT_0, MCPWM_TIMER_0, 50);
    this->currentFreq = freq;
}

int MotorHandling_getFreq()
{
    return this->currentFreq;
}


/*!*****************************************************************************
 * \fn         void MotorHandling_startServo(int freq, float duty)
 * \param      freq
 * \param      duty
 * \return     none
 *******************************************************************************/
void MotorHandling_startServo(int freq, float duty)
{
    Debug_info("Configuring servo motor (freq:%d Hz)...\n", freq);
    mcpwm_config_t pwm_configX;
    pwm_configX.frequency = freq;
    pwm_configX.cmpr_a = 0;
    pwm_configX.cmpr_b = 0;
    pwm_configX.counter_mode = MCPWM_UP_COUNTER;
    pwm_configX.duty_mode = MCPWM_DUTY_MODE_0;
    mcpwm_init(MCPWM_UNIT_1, MCPWM_TIMER_1, &pwm_configX);
    MotorHandling_forward(MCPWM_UNIT_1, MCPWM_TIMER_1, duty);
}

static void MotorHandling_hookCallbackForTimer()
{
    Gpio_toggle(STEP_MOTOR_STEP);
    Timer_reloadItTimer(this->freq);
}

void MotorHandling_hookInit(double freq, int direction)
{
    gpio_config_t gpioConfig;
    gpioConfig.pin_bit_mask = (1 << STEP_MOTOR_STEP);
    gpioConfig.mode = VR_GPIO_MODE_INPUT_OUTPUT;
    gpioConfig.pull_up_en = GPIO_PULLUP_DISABLE;
    gpioConfig.pull_down_en = GPIO_PULLDOWN_DISABLE;
    gpioConfig.intr_type = GPIO_INTR_DISABLE;
    gpio_config(&gpioConfig);
    MotorHandling_hookSetFreqAndDir(freq, direction);
    Timer_initItTimer(MotorHandling_hookCallbackForTimer);
}

void MotorHandling_hookSetFreqAndDir(double freq, int direction)
{
    this->currentFreq = (int)freq;
    MotorHandling_setDirection(direction);
    this->freq = (1.0 / (double)freq) / 2.0;
    Debug_info("Time in uS: %f\n", this->freq);
}

void MotorHandling_hookStop(void)
{
    gpio_set_level(STEP_MOTOR_STEP, 0);
    Timer_stopItTimer();
}


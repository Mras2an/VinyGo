#include "FileSystem.h"
#include "CpuDelay.h"
#include "Board.h"
#include "MotorHandling.h"
#include "Wifi.h"
#include "Ota.h"
#include "Gpio.h"
#include "Initialize.h"
#include "Lcd.h"
#include "Sensors.h"
#include "Heater.h"
#include "Midi.h"
#include "SpeedVinyl.h"
#include "ServoMotor.h"
#include "Worm.h"

/*!************************************************************************************************
 * \fn         void Initialize_init(void)
 * \param      empty
 * \return     void
 ***************************************************************************************************/
void Initialize_init(void)
{
    Fs_init();
    CpuDelay_init();
    Ota_init();
    //Wifi_saveSSIDAndPass("xxxx", "xxxxx");
    Lcd_init();
    Sensors_init();
    Heater_init();
    SpeedVinyl_init();
    ServoMotor_init();
    Worm_init();
    MotorHandling_init();
    VR_ERROR_CHECK(Gpio_configure(VACUUM_CMD_BP1, VR_GPIO_MODE_INPUT_OUTPUT, VR_GPIO_PULL_DOWN));
    VR_ERROR_CHECK(Gpio_set(VACUUM_CMD_BP1, VR_LEVEL_LOW));
    VR_ERROR_CHECK(Gpio_configure(VACUUM_CMD_BP2, VR_GPIO_MODE_INPUT_OUTPUT, VR_GPIO_PULL_DOWN));
    VR_ERROR_CHECK(Gpio_set(VACUUM_CMD_BP2, VR_LEVEL_LOW));
    Midi_init();
    //Initialize_testHardware();
}

/******************************************************************************
 * \fn       void Initialize_testHardware(void)
 * \param    void
 * \return   void
 *******************************************************************************/
void Initialize_testHardware(void)
{
    //Initialize_init();
    VR_ERROR_CHECK(Gpio_set(CMD_HEATING_12V, VR_LEVEL_HIGH));
    CpuDelay_ms(5000);
    VR_ERROR_CHECK(Gpio_set(CMD_HEATING_12V, VR_LEVEL_LOW));
    VR_ERROR_CHECK(Gpio_set(CMD_MOTOR_EN1, VR_LEVEL_HIGH));
    VR_ERROR_CHECK(Gpio_set(CMD_MOTOR_INA1, VR_LEVEL_HIGH));
    CpuDelay_ms(5000);
    VR_ERROR_CHECK(Gpio_set(CMD_MOTOR_INA1, VR_LEVEL_LOW));

    for(;;) {
        MotorHandling_startServo(50, 5);
        CpuDelay_ms(2000);
        MotorHandling_startServo(50, 7.5);
        CpuDelay_ms(2000);
        MotorHandling_start(1000, 1);
        CpuDelay_ms(2000);
        MotorHandling_start(83, 1);
    }
}

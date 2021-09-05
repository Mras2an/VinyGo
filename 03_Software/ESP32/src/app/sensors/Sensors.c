#include "Uart.h"
#include "CpuDelay.h"
#include "Bsp.h"
#include "BspConfig.h"
#include "Debug.h"
#include "AccessPoint.h"
#include "Lcd.h"
#include "MotorHandling.h"
#include "MecaFeature.h"
#include "Ota.h"
#include "Board.h"
#include "ModeAutomatic.h"
#include "ModeManual.h"
#include "ModeSystem.h"
#include "ServoMotor.h"
#include "Midi.h"
#include "SpeedVinyl.h"
#include "Heater.h"

static void Sensors_initDataTask(void);

void Sensors_init()
{
    VR_ERROR_CHECK(Gpio_configure(HEAD_DETECT, VR_GPIO_MODE_INPUT, VR_GPIO_PULL_NONE));
    VR_ERROR_CHECK(Gpio_configure(HALL_DETECT_7P, VR_GPIO_MODE_INPUT, VR_GPIO_PULL_NONE));
    VR_ERROR_CHECK(Gpio_configure(HALL_DETECT_12P, VR_GPIO_MODE_INPUT, VR_GPIO_PULL_NONE));
    VR_ERROR_CHECK(Gpio_configure(TRAY_DETECTION, VR_GPIO_MODE_INPUT, VR_GPIO_PULL_NONE));
    Sensors_initDataTask();
}

static void Sensors_TASK_DATA()
{
    uint32_t sensors[5];
    memset(sensors, 0, 5);

    for(;;) {
        Gpio_get(HEAD_DETECT, &sensors[1]);
        Gpio_get(HALL_DETECT_7P, &sensors[2]);
        Gpio_get(HALL_DETECT_12P, &sensors[3]);
        Gpio_get(TRAY_DETECTION, &sensors[4]);

        if(sensors[1]) {
            Lcd_sendWaveForm(2, 2, 100);
        } else {
            Lcd_sendWaveForm(2, 2, 140);
        }

        if(!sensors[2]) {
            Lcd_sendWaveForm(2, 0, 0);
        } else {
            Lcd_sendWaveForm(2, 0, 40);

            if(SpeedVinyl_get() == 7) {
                if(!MotorHandling_getDirection() && (Lcd_getMode() != LCD_MENU_MODE_TESTS)) {
                    MotorHandling_stop();
                    MotorHandling_hookStop();
                    Midi_sendD1CmdOff();
                    Heater_stopHeating();
                    ModeAutomatic_setModeStarted(0);
                    char tmp[20];

                    if(Lcd_getMode() == LCD_MENU_MODE_AUTOMATIC) {
                        memset(tmp, '\0', 20);
                        Heater_getValueForLcd(tmp);
                        Lcd_sendTxtCmd(strlen(tmp), "t10", tmp);
                    }

                    CpuDelay_ms(2500);
                    ServoMotor_up();
                    memset(tmp, '\0', 20);
                    CpuDelay_ms(100);
                    Midi_sendD3CmdOn();
                    ServoMotor_getUpOrDown(tmp);

                    if(strncmp(tmp, "Up", strlen("Up")) == 0)
                        MotorHandling_start(2000, 1);
                }
            }
        }

        if(!sensors[3]) {
            Lcd_sendWaveForm(2, 1, 50);
        } else {
            Lcd_sendWaveForm(2, 1, 90);

            if(SpeedVinyl_get() != 7) {
                if(!MotorHandling_getDirection()&& (Lcd_getMode() != LCD_MENU_MODE_TESTS)) {
                    MotorHandling_stop();
                    MotorHandling_hookStop();
                    Midi_sendD1CmdOff();
                    Heater_stopHeating();
                    ModeAutomatic_setModeStarted(0);
                    char tmp[20];

                    if(Lcd_getMode() == LCD_MENU_MODE_AUTOMATIC) {
                        memset(tmp, '\0', 20);
                        Heater_getValueForLcd(tmp);
                        Lcd_sendTxtCmd(strlen(tmp), "t10", tmp);
                    }

                    CpuDelay_ms(2500);
                    ServoMotor_up();
                    memset(tmp, '\0', 20);
                    CpuDelay_ms(100);
                    Midi_sendD3CmdOn();
                    ServoMotor_getUpOrDown(tmp);

                    if(strncmp(tmp, "Up", strlen("Up")) == 0)
                        MotorHandling_start(2000, 1);
                }
            }
        }

        if(sensors[4]) {
            Lcd_sendWaveForm(2, 3, 150);
        } else {
            Lcd_sendWaveForm(2, 3, 190);

            if(MotorHandling_getDirection()) {
                MotorHandling_stop();
                MotorHandling_hookStop();
            }
        }

        CpuDelay_ms(100);
    }
}

static void Sensors_initDataTask(void)
{
    OsTaskCreate(Sensors_TASK_DATA, "sensors_task", 4096, NULL, 5, NULL);
}

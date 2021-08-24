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
#include "Utils.h"
#include "Heater.h"
#include "Worm.h"
#include "ServoMotor.h"
#include "SpeedVinyl.h"

void ModeManual_init()
{
    Debug_info("\n");
    char tmp[20];
    memset(tmp, '\0', 20);
    Heater_getValueForLcd(tmp);
    Lcd_sendTxtCmd(strlen(tmp), "t10", tmp);
    memset(tmp, '\0', 20);
    SpeedVinyl_getString(tmp);
    Lcd_sendTxtCmd(strlen(tmp), "t3", tmp);
}

void ModeManual_cmdReceived(unsigned int type, unsigned int action, unsigned int len, unsigned char * data)
{
    if(strncmp((const char *)data, "HeadLeft", strlen("HeadLeft")) == 0) {
        MotorHandling_stop();
        MotorHandling_hookStop();
        int freq = MERGE_BYTES_TO_WORD(data[strlen("HeadLeft") + 1], data[strlen("HeadLeft")]);
        Debug_info("freq:%d \n", freq);

        if(freq > 0) {
            if(freq > 15) {
                MotorHandling_start(freq, 0);
            } else {
                MotorHandling_hookInit(freq, 0);
            }
        }
    }

    if(strncmp((const char *)data, "convert", strlen("convert")) == 0) {
        Debug_info("\n");
        char string[10];
        memset(string, '\0', 10);
        Worm_convertFreqToTimeInString(MERGE_BYTES_TO_WORD(data[strlen("convert") + 1], data[strlen("convert")]), string);
        Lcd_sendTxtCmd(strlen(string), "t1", string);
    }

    if(strncmp((const char *)data, "HeadStop", strlen("HeadStop")) == 0) {
        Debug_info("\n");
        MotorHandling_stop();
        MotorHandling_hookStop();
    }

    if(strncmp((const char *)data, "HeadRight", strlen("HeadRight")) == 0) {
        MotorHandling_stop();
        MotorHandling_hookStop();
        int freq = MERGE_BYTES_TO_WORD(data[strlen("HeadRight") + 1], data[strlen("HeadRight")]);
        Debug_info("freq:%d \n", freq);

        if(freq > 0) {
            if(freq > 15) {
                MotorHandling_start(freq, 1);
            } else {
                MotorHandling_hookInit(freq, 1);
            }
        }
    }

    if(strncmp((const char *)data, "UpDiam", strlen("UpDiam")) == 0) {
        Debug_info("\n");
        ServoMotor_up();
        char tmp[10];
        memset(tmp, '\0', 10);
        ServoMotor_getUpOrDown(tmp);
        Lcd_sendTxtCmd(strlen(tmp), "t0", tmp);
    }

    if(strncmp((const char *)data, "DownDiam", strlen("DownDiam")) == 0) {
        Debug_info("\n");
        ServoMotor_down();
        char tmp[10];
        memset(tmp, '\0', 10);
        ServoMotor_getUpOrDown(tmp);
        Lcd_sendTxtCmd(strlen(tmp), "t0", tmp);
    }

    if(strncmp((const char *)data, "heaterUp", strlen("heaterUp")) == 0) {
        Debug_info("\n");
        Heater_increaseValue();
        char tmp[20];
        memset(tmp, '\0', 20);
        Heater_getValueForLcd(tmp);
        Lcd_sendTxtCmd(strlen(tmp), "t10", tmp);
    }

    if(strncmp((const char *)data, "heaterDown", strlen("heaterDown")) == 0) {
        Debug_info("\n");
        Heater_decreaseValue();
        char tmp[20];
        memset(tmp, '\0', 20);
        Heater_getValueForLcd(tmp);
        Lcd_sendTxtCmd(strlen(tmp), "t10", tmp);
    }

    if(strncmp((const char *)data, "heaterStop", strlen("heaterStop")) == 0) {
        Debug_info("\n");
        char tmp[20];
        memset(tmp, '\0', 20);
        Heater_setPotvalue(0);
        Heater_getValueForLcd(tmp);
        Lcd_sendTxtCmd(strlen(tmp), "t10", tmp);
    }

    if(strncmp((const char *)data, "trmDown", strlen("trmDown")) == 0) {
        Debug_info("\n");
        SpeedVinyl_set(0);
        char tmp[10];
        memset(tmp, '\0', 10);
        SpeedVinyl_getString(tmp);
        Lcd_sendTxtCmd(strlen(tmp), "t2", tmp);
        //        VR_ERROR_CHECK(Gpio_set(CMD_MOTOR_EN1, VR_LEVEL_HIGH));
        //        VR_ERROR_CHECK(Gpio_set(CMD_MOTOR_INA1, VR_LEVEL_HIGH));
    }

    if(strncmp((const char *)data, "trmStop", strlen("trmStop")) == 0) {
        Debug_info("\n");
        //        VR_ERROR_CHECK(Gpio_set(CMD_MOTOR_EN1, VR_LEVEL_HIGH));
        //        VR_ERROR_CHECK(Gpio_set(CMD_MOTOR_INA1, VR_LEVEL_HIGH));
    }

    if(strncmp((const char *)data, "trmUp", strlen("trmUp")) == 0) {
        Debug_info("\n");
        SpeedVinyl_set(1);
        char tmp[10];
        memset(tmp, '\0', 10);
        SpeedVinyl_getString(tmp);
        Lcd_sendTxtCmd(strlen(tmp), "t2", tmp);
        //        VR_ERROR_CHECK(Gpio_set(CMD_MOTOR_EN1, VR_LEVEL_LOW));
        //        VR_ERROR_CHECK(Gpio_set(CMD_MOTOR_INA1, VR_LEVEL_LOW));
    }

    if(strncmp((const char *)data, "vacuumDown", strlen("vacuumDown")) == 0) {
        Debug_info("\n");
        VR_ERROR_CHECK(Gpio_set(VACUUM_CMD_BP1, VR_LEVEL_HIGH));
        CpuDelay_ms(500);
        VR_ERROR_CHECK(Gpio_set(VACUUM_CMD_BP1, VR_LEVEL_LOW));
    }

    if(strncmp((const char *)data, "vacuumUp", strlen("vacuumUp")) == 0) {
        Debug_info("\n");
        VR_ERROR_CHECK(Gpio_set(VACUUM_CMD_BP2, VR_LEVEL_HIGH));
        CpuDelay_ms(500);
        VR_ERROR_CHECK(Gpio_set(VACUUM_CMD_BP2, VR_LEVEL_LOW));
    }
}




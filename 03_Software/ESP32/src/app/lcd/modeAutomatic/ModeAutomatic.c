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
#include "ServoMotor.h"
#include "Utils.h"
#include "Midi.h"
#include "Heater.h"
#include "Worm.h"
#include "SpeedVinyl.h"
#include "ModeCalibration.h"

typedef struct ModeAuto_t {
    int motorFreq;
    int modeIsStarted;
} ModeAuto;

ModeAuto ModeAuto_t;
#define this (&ModeAuto_t)

OsTimerHandle_t esp32Timer;

void ModeAutomatic_init()
{
    Debug_info("\n");
    this->motorFreq = 0;
    this->modeIsStarted = 0;
    char tmp[20];
    memset(tmp, '\0', 20);
    Heater_getValueForLcd(tmp);
    Lcd_sendTxtCmd(strlen(tmp), "t10", tmp);
    memset(tmp, '\0', 20);
    SpeedVinyl_set(12);
    SpeedVinyl_getString(tmp);
    Lcd_sendTxtCmd(strlen(tmp), "t1", tmp);
    //    memset(tmp, '\0', 20);
    //    SpeedVinyl_set33or45(33);
    //    SpeedVinyl_get33or45String(tmp);
    //    Lcd_sendTxtCmd(strlen(tmp), "t2", tmp);
}

static void ModeAutomatic_stop()
{
    MotorHandling_start(ModeCalibration_getMotorValC2ForMidi(), 0);
    CpuDelay_ms(2500);
    MotorHandling_stop();
    MotorHandling_hookStop();
    CpuDelay_ms(2500);
    Midi_sendD3CmdOn();
    this->modeIsStarted = 0;
    Heater_stopHeating();
    ServoMotor_up();
    char tmp[20];
    memset(tmp, '\0', 20);
    Heater_getValueForLcd(tmp);
    Lcd_sendTxtCmd(strlen(tmp), "t10", tmp);
    memset(tmp, '\0', 20);
    ServoMotor_getUpOrDown(tmp);

    if(strncmp(tmp, "Up", strlen("Up")) == 0)
        MotorHandling_start(2000, 1);
}

static void ModeAutomatic_callback()
{
    Debug_info("\n");
    MotorHandling_stop();
}

static void ModeAutomatic_startTimer(unsigned int time)

{
    esp32Timer = OsTimerCreate("Timer", time / portTICK_PERIOD_MS, 0, 0, ModeAutomatic_callback);

    if(esp32Timer == NULL) {
        Debug_err("Create timer\n");
    }

    if(OsTimerReset(esp32Timer, 0) != pdPASS) {
        Debug_err("Reset timer\n");
    }
}

int ModeAutomatic_modeIsStarted()
{
    return this->modeIsStarted;
}

void ModeAutomatic_setModeStarted(int stop)
{
    if(stop == 0)
        this->modeIsStarted = 0;
    else
        this->modeIsStarted = 1;
}

static void ModeAutomatic_start()
{
    this->modeIsStarted = 1;
    Midi_sendD2CmdOn();
    char tmp[20];
    memset(tmp, '\0', 20);
    Heater_startHeating();

    if(this->motorFreq > 0) {
        if(this->motorFreq > 15) {
            MotorHandling_start(this->motorFreq, 0);
        } else {
            MotorHandling_hookInit(this->motorFreq, 0);
        }
    } else {
        Lcd_sendTxtCmd(strlen("You need to set a motor freq"), "g0", "You need to set a motor freq");
        Heater_stopHeating();
        memset(tmp, '\0', 20);
        Heater_getValueForLcd(tmp);
        Lcd_sendTxtCmd(strlen(tmp), "t10", tmp);
        return;
    }

    ServoMotor_down();
    ServoMotor_getUpOrDown(tmp);

    if(strncmp(tmp, "Down", strlen("Down")) == 0) {
        Midi_sendD1CmdOn(); //MIDI note D1 ON
    } else {
        MotorHandling_stop();
        MotorHandling_hookStop();
        Lcd_sendTxtCmd(strlen("The head is note down"), "g0", "The head is note down");
        Heater_stopHeating();
        memset(tmp, '\0', 20);
        Heater_getValueForLcd(tmp);
        Lcd_sendTxtCmd(strlen(tmp), "t10", tmp);
    }
}


void ModeAutomatic_cmdReceived(unsigned int type, unsigned int action, unsigned int len, unsigned char * data)
{
    if(strncmp((const char *)data, "Start", strlen("Start")) == 0) {
        Debug_info("\n");
        ModeAutomatic_start();
    }

    if(strncmp((const char *)data, "Stop", strlen("Stop")) == 0) {
        Debug_info("\n");
        ModeAutomatic_stop();
    }

    if(strncmp((const char *)data, "SpeedR", strlen("SpeedR")) == 0) {
        this->motorFreq = MERGE_BYTES_TO_WORD(data[strlen("SpeedR") + 1], data[strlen("SpeedR")]);
        char string[10];
        memset(string, '\0', 10);
        Worm_convertFreqToTimeInString(this->motorFreq, string);
        Lcd_sendTxtCmd(strlen(string), "t5", string);
    }

    if(strncmp((const char *)data, "SpeedL", strlen("SpeedL")) == 0) {
        this->motorFreq = MERGE_BYTES_TO_WORD(data[strlen("SpeedL") + 1], data[strlen("SpeedL")]);
        char string[10];
        memset(string, '\0', 10);
        Worm_convertFreqToTimeInString(this->motorFreq, string);
        Lcd_sendTxtCmd(strlen(string), "t5", string);
    }

    if(strncmp((const char *)data, "Init", strlen("Init")) == 0) {
        Debug_info("\n");
        ServoMotor_up();
        MotorHandling_start(2000, 1);
    }

    if(strncmp((const char *)data, "sizeStart", strlen("sizeStart")) == 0) {
        Debug_info("\n");
        int val = SpeedVinyl_get(), time = 0;

        if(val == 7)
            time = ModeCalibration_getValForSevenP();
        else if(val == 10)
            time = ModeCalibration_getValForTenP();
        else if(val == 12)
            time = ModeCalibration_getValForTwelveP();

        if(time != 0) {
            ServoMotor_up();
            MotorHandling_start(2000, 0);
            ModeAutomatic_startTimer(time);
        }
    }

    if(strncmp((const char *)data, "sizeDown", strlen("sizeDown")) == 0) {
        Debug_info("\n");
        SpeedVinyl_decrease();
        char tmp[10];
        memset(tmp, '\0', 10);
        SpeedVinyl_getString(tmp);
        Lcd_sendTxtCmd(strlen(tmp), "t1", tmp);
        char string[10];
        memset(string, '\0', 10);
        Worm_convertFreqToTimeInString(this->motorFreq, string);
        Lcd_sendTxtCmd(strlen(string), "t5", string);
    }

    if(strncmp((const char *)data, "sizeUp", strlen("sizeUp")) == 0) {
        Debug_info("\n");
        SpeedVinyl_increase();
        char tmp[10];
        memset(tmp, '\0', 10);
        SpeedVinyl_getString(tmp);
        Lcd_sendTxtCmd(strlen(tmp), "t1", tmp);
        char string[10];
        memset(string, '\0', 10);
        Worm_convertFreqToTimeInString(this->motorFreq, string);
        Lcd_sendTxtCmd(strlen(string), "t5", string);
    }

    if(strncmp((const char *)data, "HeadLeft", strlen("HeadLeft")) == 0) {
        Debug_info("\n");
        MotorHandling_stop();
        ServoMotor_up();
        MotorHandling_start(500, 0);
    }

    if(strncmp((const char *)data, "HeadStop", strlen("HeadStop")) == 0) {
        Debug_info("\n");
        MotorHandling_stop();
    }

    if(strncmp((const char *)data, "HeadRight", strlen("HeadRight")) == 0) {
        MotorHandling_stop();
        ServoMotor_up();
        MotorHandling_start(500, 1);
    }

    if(strncmp((const char *)data, "heaterUp", strlen("heaterUp")) == 0) {
        Debug_info("\n");

        if(this->modeIsStarted == 0)
            Heater_increaseValueAndNoStart();
        else
            Heater_increaseValue();

        char tmp[20];
        memset(tmp, '\0', 20);
        Heater_getValueForLcd(tmp);
        Lcd_sendTxtCmd(strlen(tmp), "t10", tmp);
    }

    if(strncmp((const char *)data, "heaterDown", strlen("heaterDown")) == 0) {
        Debug_info("\n");

        if(this->modeIsStarted == 0)
            Heater_decreaseValueAndNoStart();
        else
            Heater_decreaseValue();

        char tmp[20];
        memset(tmp, '\0', 20);
        Heater_getValueForLcd(tmp);
        Lcd_sendTxtCmd(strlen(tmp), "t10", tmp);
    }

    if(strncmp((const char *)data, "convert", strlen("convert")) == 0) {
        Debug_info("\n");
        this->motorFreq = MERGE_BYTES_TO_WORD(data[strlen("convert") + 1], data[strlen("convert")]);
        char string[10];
        memset(string, '\0', 10);
        Worm_convertFreqToTimeInString(MERGE_BYTES_TO_WORD(data[strlen("convert") + 1], data[strlen("convert")]), string);
        Lcd_sendTxtCmd(strlen(string), "t5", string);
    }

    //    if(strncmp((const char *)data, "trmDown", strlen("trmDown")) == 0) {
    //        Debug_info("\n");
    //        SpeedVinyl_increase33or45();
    //        char tmp[10];
    //        memset(tmp, '\0', 10);
    //        SpeedVinyl_get33or45String(tmp);
    //        Lcd_sendTxtCmd(strlen(tmp), "t2", tmp);
    //        char string[10];
    //        memset(string, '\0', 10);
    //        Worm_convertFreqToTimeInString(this->motorFreq, string);
    //        Lcd_sendTxtCmd(strlen(string), "t5", string);
    //        //        VR_ERROR_CHECK(Gpio_set(CMD_MOTOR_EN1, VR_LEVEL_HIGH));
    //        //        VR_ERROR_CHECK(Gpio_set(CMD_MOTOR_INA1, VR_LEVEL_HIGH));
    //    }
    //
    //    if(strncmp((const char *)data, "trmUp", strlen("trmUp")) == 0) {
    //        Debug_info("\n");
    //        SpeedVinyl_decrease33or45(1);
    //        char tmp[10];
    //        memset(tmp, '\0', 10);
    //        SpeedVinyl_get33or45String(tmp);
    //        Lcd_sendTxtCmd(strlen(tmp), "t2", tmp);
    //        char string[10];
    //        memset(string, '\0', 10);
    //        Worm_convertFreqToTimeInString(this->motorFreq, string);
    //        Lcd_sendTxtCmd(strlen(string), "t5", string);
    //        //        VR_ERROR_CHECK(Gpio_set(CMD_MOTOR_EN1, VR_LEVEL_LOW));
    //        //        VR_ERROR_CHECK(Gpio_set(CMD_MOTOR_INA1, VR_LEVEL_LOW));
    //    }
}

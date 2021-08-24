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
#include "FileSystem.h"
#include "Heater.h"
#include "Worm.h"
#include "Utils.h"

#define FILE_NAME_SA_SERVO  "saSer"
#define FILE_NAME_SA_MOTOR  "saMot"
#define FILE_NAME_SA_ASPI   "saAsp"
#define FILE_NAME_SA_HEATER "saHea"

void ModeSemiAuto_init()
{
    Debug_info("\n");
    char tmp[20];
    memset(tmp, '\0', 20);
    Heater_getValueForLcd(tmp);
    Lcd_sendTxtCmd(strlen(tmp), "t10", tmp);
    char * servo = Fs_read(FILE_NAME_SA_SERVO, FILE_NAME_SA_SERVO);

    if(servo != NULL)  {
        OsFree(servo);
        Lcd_sendCheckBox(strlen("1"), "c0", "1");
    } else {
        Lcd_sendCheckBox(strlen("0"), "c0", "0");
    }

    char * motor = Fs_read(FILE_NAME_SA_MOTOR, FILE_NAME_SA_MOTOR);

    if(motor != NULL)  {
        OsFree(motor);
        Lcd_sendCheckBox(strlen("1"), "c1", "1");
    } else {
        Lcd_sendCheckBox(strlen("0"), "c1", "0");
    }

    char * aspi = Fs_read(FILE_NAME_SA_ASPI, FILE_NAME_SA_ASPI);

    if(aspi != NULL)  {
        OsFree(aspi);
        Lcd_sendCheckBox(strlen("1"), "c2", "1");
    } else {
        Lcd_sendCheckBox(strlen("0"), "c2", "0");
    }

    char * heater = Fs_read(FILE_NAME_SA_HEATER, FILE_NAME_SA_HEATER);

    if(heater != NULL)  {
        OsFree(heater);
        Lcd_sendCheckBox(strlen("1"), "c3", "1");
    } else {
        Lcd_sendCheckBox(strlen("0"), "c3", "0");
    }
}


void ModeSemiAuto_cmdReceived(unsigned int type, unsigned int action, unsigned int len, unsigned char * data)
{
    if(strncmp((const char *)data, "Start", strlen("Start")) == 0) {
        Debug_info("\n");
    }

    if(strncmp((const char *)data, "Stop", strlen("Stop")) == 0) {
        Debug_info("\n");
    }

    if(strncmp((const char *)data, "convert", strlen("convert")) == 0) {
        Debug_info("\n");
        char string[10];
        memset(string, '\0', 10);
        Worm_convertFreqToTimeInString(MERGE_BYTES_TO_WORD(data[strlen("convert") + 1], data[strlen("convert")]), string);
        Lcd_sendTxtCmd(strlen(string), "t2", string);
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
}

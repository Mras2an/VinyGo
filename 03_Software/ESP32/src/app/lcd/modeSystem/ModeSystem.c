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
#include "System.h"
#include "FileSystem.h"
#include "ServoMotor.h"
#include "Worm.h"
#include "ServoMotor.h"

#define ModeSystem_pageOne   ModeSystem_init

#define FILE_NAME_BUZZER        "buzz"

void ModeSystem_init()
{
    Debug_info("\n");
    Lcd_sendTxtCmd(5, "t6", "V_"GITREV);
    char tmp[15];
    memset(tmp, '\0', 15);
    sprintf(tmp, "0x%06x", Ota_getCurrentBootAddr());
    Lcd_sendTxtCmd(strlen(tmp), "t5", tmp);
    memset(tmp, '\0', 15);
    sprintf(tmp, "%dKo", OsGetFreeHeapSize()/1024);
    Lcd_sendTxtCmd(strlen(tmp), "t3", tmp);
    memset(tmp, '\0', 15);
    sprintf(tmp, "%.2fC", System_getCpuTemperature());
    Lcd_sendTxtCmd(strlen(tmp), "t4", tmp);

    if(System_isWifiEnable() == 0) {
        Lcd_sendTxtCmd(strlen("Disable"), "t7", "Disable");
        Lcd_sendImgCmd(strlen("32"), "p6", "32");
    } else {
        Lcd_sendTxtCmd(strlen("IP: 192.168.52.1"), "t7", "IP: 192.168.52.1");
        Lcd_sendImgCmd(strlen("26"), "p6", "26");
    }
}

void ModeSystem_pageTwo(void)
{
    char * buzzer = Fs_read(FILE_NAME_BUZZER, FILE_NAME_BUZZER);

    if(buzzer != NULL)  {
        OsFree(buzzer);
        Lcd_sendCheckBox(strlen("1"), "c0", "1");
    } else {
        Lcd_sendCheckBox(strlen("0"), "c0", "0");
    }

    char * wrom = Fs_read(FILE_NAME_WROM, FILE_NAME_WROM);

    if(wrom != NULL)  {
        OsFree(wrom);
        Lcd_sendCheckBox(strlen("1"), "c1", "1");
    } else {
        Lcd_sendCheckBox(strlen("0"), "c1", "0");
    }

    char * servo = Fs_read(FILE_NAME_SERVO_MOTOR, FILE_NAME_SERVO_MOTOR);

    if(servo != NULL)  {
        OsFree(servo);
        Lcd_sendCheckBox(strlen("1"), "c2", "1");
    } else {
        Lcd_sendCheckBox(strlen("0"), "c2", "0");
    }
}

void ModeSystem_cmdReceived(unsigned int type, unsigned int action, unsigned int len, unsigned char * data)
{
    if(strncmp((const char *)data, "Next", strlen("Next")) == 0) {
        Debug_info("\n");
        ModeSystem_pageTwo();
    }

    if(strncmp((const char *)data, "Previous", strlen("Previous")) == 0) {
        Debug_info("\n");
        ModeSystem_pageOne();
    }

    if(strncmp((const char *)data, "Init", strlen("Init")) == 0) {
        Debug_info("\n");
        ServoMotor_up();
        MotorHandling_start(2000, 1);
    }

    if(strncmp((const char *)data, "Buzzer", strlen("Buzzer")) == 0) {
        Debug_info("\n");
        char * buzzer = Fs_read(FILE_NAME_BUZZER, FILE_NAME_BUZZER);

        if(buzzer != NULL)  {
            Fs_delete_all(FILE_NAME_BUZZER);
            OsFree(buzzer);
        } else {
            Fs_write(FILE_NAME_BUZZER, "true", FILE_NAME_BUZZER);
        }
    }

    if(strncmp((const char *)data, "Worm", strlen("Worm")) == 0) {
        Debug_info("\n");
        Worm_toggleValue();
    }

    if(strncmp((const char *)data, "Servo", strlen("Servo")) == 0) {
        Debug_info("\n");
        ServoMotor_toggleDirection();
    }
}




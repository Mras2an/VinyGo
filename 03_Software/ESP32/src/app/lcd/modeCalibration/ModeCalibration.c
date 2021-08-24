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

typedef struct Calibration_t {
    int sevenP;
    int tenP;
    int twelveP;
    float srvUp;
    float srvDown;
    int C1;
    int C2;
} Calibration;

Calibration Calibration_t;
#define this (&Calibration_t)

#define FILE_NAME_7P_VAL        "v7p"
#define FILE_NAME_10P_VAL       "v10p"
#define FILE_NAME_12P_VAL       "v12p"
#define FILE_NAME_SRV_UP_VAL    "vSrvU"
#define FILE_NAME_SRV_DOWN_VAL  "vSrvD"
#define FILE_NAME_C1_MIDI_VAL   "C1mid"
#define FILE_NAME_C2_MIDI_VAL   "C2mid"

typedef enum {
    TIMER_IDEL,
    TIMER_INCREASE,
    TIMER_DECREASE,
    TIMER_PAUSE,

} TimerState;
static int timerCounter = TIMER_IDEL;
static int counter = 0;
static int run;

static void ModeCalibration_initTask(void);

#define ModeCalibration_pageOne   ModeCalibration_init

void ModeCalibration_init()
{
    this->sevenP = 0;
    this->tenP = 0;
    this->twelveP = 0;
    Debug_info("\n");
    char tmp[20];
    memset(tmp, '\0', 20);
    char * speedSeP = Fs_read(FILE_NAME_7P_VAL, FILE_NAME_7P_VAL);

    if(speedSeP != NULL)  {
        Debug_info("speedSeP:%s\n", speedSeP);
        Lcd_sendTxtCmd(strlen(speedSeP), "t8", speedSeP);
        this->sevenP = atoi(speedSeP);
        OsFree(speedSeP);
    } else {
        Lcd_sendTxtCmd(strlen("0"), "t8", "0");
    }

    char * speedTeP = Fs_read(FILE_NAME_10P_VAL, FILE_NAME_10P_VAL);

    if(speedTeP != NULL)  {
        Debug_info("speedTeP:%s\n", speedTeP);
        Lcd_sendTxtCmd(strlen(speedTeP), "t7", speedTeP);
        this->tenP = atoi(speedTeP);
        OsFree(speedTeP);
    } else {
        Lcd_sendTxtCmd(strlen("0"), "t7", "0");
    }

    char * speedTwP = Fs_read(FILE_NAME_12P_VAL, FILE_NAME_12P_VAL);

    if(speedTwP != NULL)  {
        Debug_info("speedTwP:%s\n", speedTwP);
        Lcd_sendTxtCmd(strlen(speedTwP), "t6", speedTwP);
        this->twelveP = atoi(speedTwP);
        OsFree(speedTwP);
    } else {
        Lcd_sendTxtCmd(strlen("0"), "t6", "0");
    }

    run = 1;
    timerCounter = TIMER_IDEL;
    ModeCalibration_initTask();
}

void ModeCalibration_pageTow()
{
    this->srvDown = 7.5;
    this->srvUp = 5.0;
    this->C1 = 150;
    this->C2 = 1000;
    char * srvUp = Fs_read(FILE_NAME_SRV_UP_VAL, FILE_NAME_SRV_UP_VAL);

    if(srvUp != NULL)  {
        Lcd_sendTxtCmd(strlen(srvUp), "t9", srvUp);
        this->srvUp = atof(srvUp);
        OsFree(srvUp);
    } else {
        Lcd_sendTxtCmd(strlen("5.0"), "t9", "5.0");
    }

    char * srvDown = Fs_read(FILE_NAME_SRV_DOWN_VAL, FILE_NAME_SRV_DOWN_VAL);

    if(srvDown != NULL)  {
        Lcd_sendTxtCmd(strlen(srvDown), "t10", srvDown);
        this->srvDown = atof(srvDown);
        OsFree(srvDown);
    } else {
        Lcd_sendTxtCmd(strlen("7.5"), "t10", "7.5");
    }

    char * c1midi = Fs_read(FILE_NAME_C1_MIDI_VAL, FILE_NAME_C1_MIDI_VAL);

    if(c1midi != NULL)  {
        Lcd_sendTxtCmd(strlen(c1midi), "t32", c1midi);
        this->C1 = atoi(c1midi);
        OsFree(c1midi);
    } else {
        Lcd_sendTxtCmd(strlen("150"), "t32", "150");
    }

    char * c2midi = Fs_read(FILE_NAME_C2_MIDI_VAL, FILE_NAME_C2_MIDI_VAL);

    if(c2midi != NULL)  {
        Lcd_sendTxtCmd(strlen(c2midi), "t38", c2midi);
        this->C2 = atoi(c2midi);
        OsFree(c2midi);
    } else {
        Lcd_sendTxtCmd(strlen("1000"), "t38", "1000");
    }
}

float ModeCalibration_getValForServoUp()
{
    float val = 5.0;
    char * srvUp = Fs_read(FILE_NAME_SRV_UP_VAL, FILE_NAME_SRV_UP_VAL);

    if(srvUp != NULL)  {
        val = atof(srvUp);
        Debug_info("%f\n", val);
        OsFree(srvUp);
    }

    return val;
}

float ModeCalibration_getValForServoDown()
{
    float val = 7.5;
    char * srvDown = Fs_read(FILE_NAME_SRV_DOWN_VAL, FILE_NAME_SRV_DOWN_VAL);

    if(srvDown != NULL)  {
        val = atof(srvDown);
        Debug_info("%f\n", val);
        OsFree(srvDown);
    }

    return val;
}

unsigned int ModeCalibration_getValForSevenP()
{
    unsigned int val = 0;
    char * speedSeP = Fs_read(FILE_NAME_7P_VAL, FILE_NAME_7P_VAL);

    if(speedSeP != NULL)  {
        val = atoi(speedSeP);
        OsFree(speedSeP);
    }

    return val;
}

unsigned int ModeCalibration_getValForTenP()
{
    unsigned int val = 0;
    char * speedTeP = Fs_read(FILE_NAME_10P_VAL, FILE_NAME_10P_VAL);

    if(speedTeP != NULL)  {
        val = atoi(speedTeP);
        OsFree(speedTeP);
    }

    return val;
}

unsigned int ModeCalibration_getValForTwelveP()
{
    unsigned int val = 0;
    char * speedTwP = Fs_read(FILE_NAME_12P_VAL, FILE_NAME_12P_VAL);

    if(speedTwP != NULL)  {
        val = atoi(speedTwP);
        OsFree(speedTwP);
    }

    return val;
}

unsigned int ModeCalibration_getMotorValC1ForMidi()
{
    unsigned int val = 150;
    char * c1midi = Fs_read(FILE_NAME_C1_MIDI_VAL, FILE_NAME_C1_MIDI_VAL);

    if(c1midi != NULL)  {
        val = atoi(c1midi);
        OsFree(c1midi);
    }

    return val;
}

unsigned int ModeCalibration_getMotorValC2ForMidi()
{
    unsigned int val = 1000;
    char * c2midi = Fs_read(FILE_NAME_C2_MIDI_VAL, FILE_NAME_C2_MIDI_VAL);

    if(c2midi != NULL)  {
        val = atoi(c2midi);
        OsFree(c2midi);
    }

    return val;
}


static void ModeCalibration_TASK_DATA()
{
    while(run) {
        CpuDelay_ms(1);

        switch(timerCounter) {
            case TIMER_PAUSE:
                break;

            case TIMER_DECREASE:
                counter--;
                break;

            case TIMER_INCREASE:
                counter++;
                break;

            default:
                counter = 0;
        }
    }

    OsTaskDelete(NULL);
}

static void ModeCalibration_initTask(void)
{
    OsTaskCreate(ModeCalibration_TASK_DATA, "timer", 4096, NULL, 5, NULL);
}

void ModeCalibration_cmdReceived(unsigned int type, unsigned int action, unsigned int len, unsigned char * data)
{
    if(strncmp((const char *)data, "Next", strlen("Next")) == 0) {
        Debug_info("\n");
        ModeCalibration_pageTow();
    }

    if(strncmp((const char *)data, "Previous", strlen("Previous")) == 0) {
        Debug_info("\n");
        ModeCalibration_pageOne();
    }

    if(strncmp((const char *)data, "Save", strlen("Save")) == 0) {
        char tmp[10];
        memset(tmp, '\0', 10);
        sprintf(tmp, "%d", this->sevenP);
        Debug_info("tmp:%s\n", tmp);
        Fs_write(FILE_NAME_7P_VAL, tmp, FILE_NAME_7P_VAL);
        memset(tmp, '\0', 10);
        sprintf(tmp, "%d", this->tenP);
        Fs_write(FILE_NAME_10P_VAL, tmp, FILE_NAME_10P_VAL);
        memset(tmp, '\0', 10);
        sprintf(tmp, "%d", this->twelveP);
        Fs_write(FILE_NAME_12P_VAL, tmp, FILE_NAME_12P_VAL);
        Debug_info("\n");
    }

    if(strncmp((const char *)data, "Store", strlen("Store")) == 0) {
        char tmp[10];
        memset(tmp, '\0', 10);
        sprintf(tmp, "%0.1f", this->srvUp);
        Fs_write(FILE_NAME_SRV_UP_VAL, tmp, FILE_NAME_SRV_UP_VAL);
        memset(tmp, '\0', 10);
        sprintf(tmp, "%0.1f", this->srvDown);
        Fs_write(FILE_NAME_SRV_DOWN_VAL, tmp, FILE_NAME_SRV_DOWN_VAL);
        memset(tmp, '\0', 10);
        sprintf(tmp, "%d", this->C1);
        Fs_write(FILE_NAME_C1_MIDI_VAL, tmp, FILE_NAME_C1_MIDI_VAL);
        memset(tmp, '\0', 10);
        sprintf(tmp, "%d", this->C2);
        Fs_write(FILE_NAME_C2_MIDI_VAL, tmp, FILE_NAME_C2_MIDI_VAL);
        Debug_info("\n");
    }

    if(strncmp((const char *)data, "Menu", strlen("Menu")) == 0) {
        Debug_info("\n");
        run = 0;
    }

    if(strncmp((const char *)data, "12p+", strlen("12p+")) == 0) {
        Debug_info("\n");
        MotorHandling_stop();
        MotorHandling_start(2000, 0);
        timerCounter = TIMER_INCREASE;
    }

    if(strncmp((const char *)data, "12p-", strlen("12p-")) == 0) {
        Debug_info("\n");
        MotorHandling_stop();
        MotorHandling_start(2000, 1);
        timerCounter = TIMER_DECREASE;
    }

    if(strncmp((const char *)data, "12pS", strlen("12pS")) == 0) {
        timerCounter = TIMER_PAUSE;
        MotorHandling_stop();
        Debug_info(" time: %d\n", counter);
        this->twelveP = counter;
        char tmp[10];
        memset(tmp, '\0', 10);
        sprintf(tmp, "%d", this->twelveP);
        Lcd_sendTxtCmd(strlen(tmp), "t6", tmp);
    }

    if(strncmp((const char *)data, "12pI", strlen("12pI")) == 0) {
        Debug_info("\n");
        timerCounter = TIMER_IDEL;
        MotorHandling_stop();
        MotorHandling_start(2000, 1);
        Lcd_sendTxtCmd(strlen("0"), "t6", "0");
    }

    if(strncmp((const char *)data, "10p+", strlen("10p+")) == 0) {
        Debug_info("\n");
        MotorHandling_stop();
        MotorHandling_start(2000, 0);
        timerCounter = TIMER_INCREASE;
    }

    if(strncmp((const char *)data, "10p-", strlen("10p-")) == 0) {
        Debug_info("\n");
        MotorHandling_stop();
        MotorHandling_start(2000, 1);
        timerCounter = TIMER_DECREASE;
    }

    if(strncmp((const char *)data, "10pS", strlen("10pS")) == 0) {
        timerCounter = TIMER_PAUSE;
        MotorHandling_stop();
        Debug_info(" time: %d\n", counter);
        this->tenP = counter;
        char tmp[10];
        memset(tmp, '\0', 10);
        sprintf(tmp, "%d", this->tenP);
        Lcd_sendTxtCmd(strlen(tmp), "t7", tmp);
    }

    if(strncmp((const char *)data, "10pI", strlen("10pI")) == 0) {
        Debug_info("\n");
        timerCounter = TIMER_IDEL;
        MotorHandling_stop();
        MotorHandling_start(2000, 1);
        Lcd_sendTxtCmd(strlen("0"), "t7", "0");
    }

    if(strncmp((const char *)data, "07p+", strlen("07p+")) == 0) {
        Debug_info("\n");
        MotorHandling_stop();
        MotorHandling_start(2000, 0);
        timerCounter = TIMER_INCREASE;
    }

    if(strncmp((const char *)data, "07p-", strlen("07p-")) == 0) {
        Debug_info("\n");
        MotorHandling_stop();
        MotorHandling_start(2000, 1);
        timerCounter = TIMER_DECREASE;
    }

    if(strncmp((const char *)data, "07pS", strlen("07pS")) == 0) {
        timerCounter = TIMER_PAUSE;
        MotorHandling_stop();
        Debug_info(" time: %d\n", counter);
        this->sevenP = counter;
        char tmp[10];
        memset(tmp, '\0', 10);
        sprintf(tmp, "%d", this->sevenP);
        Lcd_sendTxtCmd(strlen(tmp), "t8", tmp);
    }

    if(strncmp((const char *)data, "07pI", strlen("07pI")) == 0) {
        Debug_info("\n");
        timerCounter = TIMER_IDEL;
        MotorHandling_stop();
        MotorHandling_start(2000, 1);
        Lcd_sendTxtCmd(strlen("0"), "t8", "0");
    }

    if(strncmp((const char *)data, "Up-", strlen("Up-")) == 0) {
        Debug_info("\n");

        if(this->srvUp < 15.0)
            this->srvUp += 0.1;

        char tmp[10];
        memset(tmp, '\0', 10);
        sprintf(tmp, "%0.1f", this->srvUp);
        Lcd_sendTxtCmd(strlen(tmp), "t9", tmp);
        MotorHandling_startServo(50, this->srvUp);
    }

    if(strncmp((const char *)data, "Up+", strlen("Up+")) == 0) {
        Debug_info("\n");

        if(this->srvUp > 5)
            this->srvUp -= 0.1;

        char tmp[10];
        memset(tmp, '\0', 10);
        sprintf(tmp, "%0.1f", this->srvUp);
        Lcd_sendTxtCmd(strlen(tmp), "t9", tmp);
        MotorHandling_startServo(50, this->srvUp);
    }

    if(strncmp((const char *)data, "Do-", strlen("Do-")) == 0) {
        Debug_info("\n");

        if(this->srvDown < 15.0)
            this->srvDown += 0.1;

        char tmp[10];
        memset(tmp, '\0', 10);
        sprintf(tmp, "%0.1f", this->srvDown);
        Lcd_sendTxtCmd(strlen(tmp), "t10", tmp);
        MotorHandling_startServo(50, this->srvDown);
    }

    if(strncmp((const char *)data, "Do+", strlen("Do+")) == 0) {
        Debug_info("\n");

        if(this->srvDown > 5)
            this->srvDown -= 0.1;

        char tmp[10];
        memset(tmp, '\0', 10);
        sprintf(tmp, "%0.1f", this->srvDown);
        Lcd_sendTxtCmd(strlen(tmp), "t10", tmp);
        MotorHandling_startServo(50, this->srvDown);
    }

    if(strncmp((const char *)data, "C1+", strlen("C1+")) == 0) {
        Debug_info("\n");
        this->C1 += 10;

        if(this->C1 > 500)
            this->C1 = 500;

        char tmp[10];
        memset(tmp, '\0', 10);
        sprintf(tmp, "%d", this->C1);
        Lcd_sendTxtCmd(strlen(tmp), "t32", tmp);
    }

    if(strncmp((const char *)data, "C1-", strlen("C1-")) == 0) {
        Debug_info("\n");
        this->C1 -= 10;

        if(this->C1 < 0)
            this->C1 = 0;

        char tmp[10];
        memset(tmp, '\0', 10);
        sprintf(tmp, "%d", this->C1);
        Lcd_sendTxtCmd(strlen(tmp), "t32", tmp);
    }

    if(strncmp((const char *)data, "C2+", strlen("C2+")) == 0) {
        Debug_info("\n");
        this->C2 += 50;

        if(this->C2 > 2000)
            this->C2 = 2000;

        char tmp[10];
        memset(tmp, '\0', 10);
        sprintf(tmp, "%d", this->C2);
        Lcd_sendTxtCmd(strlen(tmp), "t38", tmp);
    }

    if(strncmp((const char *)data, "C2-", strlen("C2-")) == 0) {
        Debug_info("\n");
        this->C2 -= 50;

        if(this->C2 < 0)
            this->C2 = 0;

        char tmp[10];
        memset(tmp, '\0', 10);
        sprintf(tmp, "%d", this->C2);
        Lcd_sendTxtCmd(strlen(tmp), "t38", tmp);
    }
}

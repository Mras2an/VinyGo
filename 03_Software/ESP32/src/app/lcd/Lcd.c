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
#include "ModeSemiAuto.h"
#include "ModeTests.h"
#include "ModeUpdate.h"
#include "ModeCalibration.h"

#define MAX_LCD_DATA 100
typedef struct Lcd_t {
    int counter;
    unsigned char type;
    unsigned char action;
    unsigned int len;
    unsigned char data[MAX_LCD_DATA];
} Lcd;

Lcd Lcd_t;
#define this (&Lcd_t)

static void Lcd_initDataTask(void);

typedef enum {
    LCD_STATE_BEGIN,
    LCD_STATE_TYPE,
    LCD_STATE_ACTION,
    LCD_STATE_LEN,
    LCD_STATE_DATA,
    LCD_STATE_END,
} LcdState;



static LcdMode eMenuMode = LCD_MENU_MODE_IDL_MENU;


void Lcd_init()
{
    this->action = 0;
    memset(this->data, '\0', MAX_LCD_DATA);
    this->type = 0;
    this->len = 0;
    this->counter = 0;
    Uart_init(2, 9600, RX_LCD_3V3, TX_LCD_3V3);
    CpuDelay_ms(200);
    Lcd_initDataTask();
}

static void Lcd_cmdReceived()
{
    Debug_info("Type: %x\n", this->type);
    Debug_info("Action: %x\n", this->action);
    Debug_info("Len: %d\n", strlen((const char *)this->data));
    Debug_info("Data: %s\n", this->data);

    if(strncmp((const char *)this->data, "Automatic", strlen("Automatic")) == 0) {
        eMenuMode = LCD_MENU_MODE_AUTOMATIC;
        ModeAutomatic_init();
    }

    if(strncmp((const char *)this->data, "Manual", strlen("Manual")) == 0) {
        eMenuMode = LCD_MENU_MODE_MANUAL;
        ModeManual_init();
    }

    if(strncmp((const char *)this->data, "System", strlen("System")) == 0) {
        eMenuMode = LCD_MENU_MODE_SYSTEM;
        ModeSystem_init();
    }

    if(strncmp((const char *)this->data, "SemiAuto", strlen("SemiAuto")) == 0) {
        eMenuMode = LCD_MENU_MODE_SEMIAUTO;
        ModeSemiAuto_init();
    }

    if(strncmp((const char *)this->data, "Tests", strlen("Tests")) == 0) {
        eMenuMode = LCD_MENU_MODE_TESTS;
        ModeTests_init();
    }

    if(strncmp((const char *)this->data, "Update", strlen("Update")) == 0) {
        eMenuMode = LCD_MENU_MODE_UPDATE;
        ModeUpdate_init();
    }

    if(strncmp((const char *)this->data, "Calibration", strlen("Calibration")) == 0) {
        eMenuMode = LCD_MENU_MODE_CALIBRATION;
        ModeCalibration_init();
    }

    switch(eMenuMode) {
        case(LCD_MENU_MODE_AUTOMATIC): {
            ModeAutomatic_cmdReceived(this->type, this->action, this->len, this->data);
        }
        break;

        case(LCD_MENU_MODE_MANUAL): {
            ModeManual_cmdReceived(this->type, this->action, this->len, this->data);
        }
        break;

        case(LCD_MENU_MODE_SYSTEM): {
            ModeSystem_cmdReceived(this->type, this->action, this->len, this->data);
        }
        break;

        case(LCD_MENU_MODE_SEMIAUTO): {
            ModeSemiAuto_cmdReceived(this->type, this->action, this->len, this->data);
        }
        break;

        case(LCD_MENU_MODE_TESTS): {
            ModeTests_cmdReceived(this->type, this->action, this->len, this->data);
        }
        break;

        case(LCD_MENU_MODE_UPDATE): {
            ModeUpdate_cmdReceived(this->type, this->action, this->len, this->data);
        }
        break;

        case(LCD_MENU_MODE_CALIBRATION): {
            ModeCalibration_cmdReceived(this->type, this->action, this->len, this->data);
        }
        break;

        default:
            ;
    }
}

static void Lcd_aByteIsReceived(unsigned char buffer)
{
    static LcdState eState = LCD_STATE_BEGIN;

    switch(eState) {
        case(LCD_STATE_BEGIN): {
            if(buffer == 0x55) {
                memset(this->data, '\0', MAX_LCD_DATA);
                eState = LCD_STATE_TYPE;
            }
        }
        break;

        case(LCD_STATE_TYPE): {
            this->type = buffer;
            eState = LCD_STATE_ACTION;
        }
        break;

        case(LCD_STATE_ACTION): {
            this->action = buffer;
            eState = LCD_STATE_LEN;
        }
        break;

        case(LCD_STATE_LEN): {
            this->len = buffer;

            if(this->len < MAX_LCD_DATA) {
                this->counter = 0;
                eState = LCD_STATE_DATA;
            } else
                eState = LCD_STATE_BEGIN;
        }
        break;

        case(LCD_STATE_DATA): {
            if((this->len - 1) == this->counter)
                eState = LCD_STATE_END;

            this->data[this->counter++] = buffer;
        }
        break;

        case(LCD_STATE_END): {
            if(buffer == 0x54) {
                Lcd_cmdReceived();
            }

            eState = LCD_STATE_BEGIN;
        }
        break;

        default:
            ;
    }
}

static void Lcd_TASK_DATA()
{
    unsigned char data[2];

    for(;;) {
        if(Uart_read(2, 1, data) != -1) {
            Lcd_aByteIsReceived(data[0]);
        }
    }
}

static void Lcd_initDataTask(void)
{
    OsTaskCreate(Lcd_TASK_DATA, "lcd_uart_task", 4096, NULL, 4, NULL);
}

void Lcd_sendOtaInfo(int len, const char * data)
{
    char tmp[len + 20];
    memset(tmp, '\0', len + 20);
    memcpy(tmp, "t8.txt=\"", strlen("t7.txt=\""));
    strcat(tmp, data);
    strcat(tmp, "\"");
    int size = strlen(tmp);
    tmp[size] = 0xff;
    tmp[size+1] = 0xff;
    tmp[size+2] = 0xff;
    Uart_write(2, size + 3, tmp);
}

void Lcd_sendTxtCmd(int len, const char * id, const char * data)
{
    char tmp[len + 20];
    memset(tmp, '\0', len + 20);
    memcpy(tmp, id, strlen(id));
    strcat(tmp, ".txt=\"");
    strcat(tmp, data);
    strcat(tmp, "\"");
    int size = strlen(tmp);
    tmp[size] = 0xff;
    tmp[size+1] = 0xff;
    tmp[size+2] = 0xff;
    Uart_write(2, size + 3, tmp);
}

void Lcd_sendCheckBox(int len, const char * id, const char * data)
{
    char tmp[len + 20];
    memset(tmp, '\0', len + 20);
    memcpy(tmp, id, strlen(id));
    strcat(tmp, ".val=");
    strcat(tmp, data);
    int size = strlen(tmp);
    tmp[size] = 0xff;
    tmp[size+1] = 0xff;
    tmp[size+2] = 0xff;
    Debug_info("%s\n", tmp);
    Uart_write(2, size + 3, tmp);
}

void Lcd_sendImgCmd(int len, const char * id, const char * data)
{
    char tmp[len + 20];
    memset(tmp, '\0', len + 20);
    memcpy(tmp, id, strlen(id));
    strcat(tmp, ".pic=");
    strcat(tmp, data);
    int size = strlen(tmp);
    tmp[size] = 0xff;
    tmp[size+1] = 0xff;
    tmp[size+2] = 0xff;
    Uart_write(2, size + 3, tmp);
}

void Lcd_sendWaveForm(int id, int ch, int data)
{
    if(eMenuMode != LCD_MENU_MODE_TESTS)
        return;

    char tmp[50];
    memset(tmp, '\0', 50);
    sprintf(tmp, "add %d,%d,%d", id, ch, data);
    int size = strlen(tmp);
    tmp[size] = 0xff;
    tmp[size+1] = 0xff;
    tmp[size+2] = 0xff;
    Uart_write(2, size + 3, tmp);
}

LcdMode Lcd_getMode()
{
    return eMenuMode;
}

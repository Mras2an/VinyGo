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
#include "Spi.h"
#include "FileSystem.h"

#define FILE_NAME_HEATER_INFO  "heVal"

typedef struct Heater_t {
    int heaterValue;
} Heater;

Heater Heater_t;
#define this (&Heater_t)

void Heater_init()
{
    VR_ERROR_CHECK(
        Gpio_configure(CMD_HEATING_12V, VR_GPIO_MODE_INPUT_OUTPUT, VR_GPIO_PULL_NONE));
    VR_ERROR_CHECK(Gpio_set(CMD_HEATING_12V, VR_LEVEL_LOW));
    VR_ERROR_CHECK(
        Gpio_configure(MCP_RES_CS, VR_GPIO_MODE_INPUT_OUTPUT, VR_GPIO_PULL_NONE));
    VR_ERROR_CHECK(Gpio_set(MCP_RES_CS, VR_LEVEL_LOW));
    Spi_init();
    this->heaterValue = 0;
}

void Heater_setPotvalue(int value)
{
    if(value > 0) {
        int address = 0x00;
        VR_ERROR_CHECK(Gpio_set(MCP_RES_CS, VR_LEVEL_LOW));
        Spi_writeByte(address);
        Spi_writeByte(value);
        VR_ERROR_CHECK(Gpio_set(MCP_RES_CS, VR_LEVEL_HIGH));
        VR_ERROR_CHECK(Gpio_set(CMD_HEATING_12V, VR_LEVEL_HIGH));
    } else {
        this->heaterValue = 0;
        VR_ERROR_CHECK(Gpio_set(CMD_HEATING_12V, VR_LEVEL_LOW));
    }
}

//void Heater_saveValue(int value)
//{
//    //if((value > 0) && (value < 200)) {
//        char tmp[10];
//        memset(tmp, '\0', 10);
//        sprintf(tmp, "%d", value);
//        Fs_write(FILE_NAME_HEATER_INFO, tmp, FILE_NAME_HEATER_INFO);
//    //}
//}

//float Heater_readValue(void)
//{
//    char * val = Fs_read(FILE_NAME_HEATER_INFO, FILE_NAME_HEATER_INFO);
//    float heaterValue = 0.0;
//
//    if(val != NULL)  {
//        heaterValue = atof(val);
//        Debug_info("%f\n", heaterValue);
//        OsFree(val);
//    }
//
//    return heaterValue;
//}

int Heater_getValue(void)
{
    return this->heaterValue;
}

void Heater_setValue(int value)
{
    this->heaterValue = value;
}

void Heater_getValueForLcd(char * buffer)
{
    //
    float vOut = 0;

    switch(this->heaterValue) {
        case 1:
            vOut = 1.507;
            break;

        case 2:
            vOut = 1.926;
            break;

        case 3:
            vOut = 2.346;
            break;

        case 4:
            vOut = 2.766;
            break;

        case 5:
            vOut = 3.186;
            break;

        case 6:
            vOut = 3.555;
            break;

        case 7:
            vOut = 3.925;
            break;

        case 8:
            vOut = 4.522;
            break;

        case 9:
            vOut = 5.12;
            break;

        default:
            ;
    }

    float iOut = vOut/10.5;
    float wOut = vOut * iOut;
    sprintf(buffer, "%.2fW %.2fV %.3fA", wOut, vOut, iOut);
}

void Heater_increaseValueAndNoStart(void)
{
    this->heaterValue++;

    if(this->heaterValue > 9)
        this->heaterValue = 0;
}

void Heater_decreaseValueAndNoStart(void)
{
    this->heaterValue--;

    if(this->heaterValue < 0)
        this->heaterValue = 0;
}

void Heater_stopHeating(void)
{
    Heater_setPotvalue(0);
}

void Heater_startHeating(void)
{
    switch(this->heaterValue) {
        case 0:
            Heater_setPotvalue(this->heaterValue);
            break;

        case 1:
            Heater_setPotvalue(254);
            break;

        case 2:
            Heater_setPotvalue(247);
            break;

        case 3:
            Heater_setPotvalue(239);
            break;

        case 4:
            Heater_setPotvalue(232);
            break;

        case 5:
            Heater_setPotvalue(224);
            break;

        case 6:
            Heater_setPotvalue(217);
            break;

        case 7:
            Heater_setPotvalue(209);
            break;

        case 8:
            Heater_setPotvalue(194);
            break;

        case 9:
            Heater_setPotvalue(180);
            break;

        default: {
            this->heaterValue = 0;
            Heater_setPotvalue(this->heaterValue);
        }
    }
}

void Heater_increaseValue(void)
{
    this->heaterValue++;

    switch(this->heaterValue) {
        case 0:
            Heater_setPotvalue(this->heaterValue);
            break;

        case 1:
            Heater_setPotvalue(254);
            break;

        case 2:
            Heater_setPotvalue(247);
            break;

        case 3:
            Heater_setPotvalue(239);
            break;

        case 4:
            Heater_setPotvalue(232);
            break;

        case 5:
            Heater_setPotvalue(224);
            break;

        case 6:
            Heater_setPotvalue(217);
            break;

        case 7:
            Heater_setPotvalue(209);
            break;

        case 8:
            Heater_setPotvalue(194);
            break;

        case 9:
            Heater_setPotvalue(180);
            break;

        default: {
            this->heaterValue = 0;
            Heater_setPotvalue(this->heaterValue);
        }
    }
}
void Heater_decreaseValue(void)
{
    this->heaterValue--;

    switch(this->heaterValue) {
        case 0:
            Heater_setPotvalue(this->heaterValue);
            break;

        case 1:
            Heater_setPotvalue(254);
            break;

        case 2:
            Heater_setPotvalue(247);
            break;

        case 3:
            Heater_setPotvalue(239);
            break;

        case 4:
            Heater_setPotvalue(232);
            break;

        case 5:
            Heater_setPotvalue(224);
            break;

        case 6:
            Heater_setPotvalue(217);
            break;

        case 7:
            Heater_setPotvalue(209);
            break;

        case 8:
            Heater_setPotvalue(194);
            break;

        case 9:
            Heater_setPotvalue(180);
            break;

        default: {
            this->heaterValue = 0;
            Heater_setPotvalue(this->heaterValue);
        }
    }
}

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

void ModeUpdate_init()
{
    Debug_info("\n");
}

void ModeUpdate_cmdReceived(unsigned int type, unsigned int action, unsigned int len, unsigned char * data)
{
    if(strncmp((const char *)data, "Update", strlen("Update")) == 0) {
        Debug_info("\n");
        AccessPoint_findSSIDAndPass();
        AccessPoint_init();
    }
}

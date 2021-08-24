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
#include "Midi.h"
#include "Board.h"

void ModeTests_init()
{
    Debug_info("\n");
}

void ModeTests_cmdReceived(unsigned int type, unsigned int action, unsigned int len, unsigned char * data)
{
    if(strncmp((const char *)data, "MidiOut", strlen("MidiOut")) == 0) {
        Debug_info("\n");
        static char tmp[3] = {0x90, 0x14, 0x7F};
        Midi_sendCmd(3, tmp);
        CpuDelay_ms(1000);
        static char tmp2[3] = {0x80, 0x14, 0x7F};
        Midi_sendCmd(3, tmp2);
    }

    if(strncmp((const char *)data, "D2", strlen("D2")) == 0) {
        Midi_sendD2CmdOn();
        CpuDelay_ms(200);
        Midi_sendD2CmdOff();
    }

    if(strncmp((const char *)data, "D3", strlen("D3")) == 0) {
        Midi_sendD3CmdOn();
        CpuDelay_ms(200);
        Midi_sendD3CmdOff();
    }
}

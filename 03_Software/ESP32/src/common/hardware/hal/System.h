#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#ifdef ESP32
    #include "Esp32.h"
    #define System_init               Esp32_init
    #define System_getIp              Esp32_getIp
    #define System_delete             Esp32_delete
    #define System_saveIp             Esp32_saveIp
    #define System_getMask            Esp32_getMask
    #define System_saveMask           Esp32_saveMask
    #define System_getGw              Esp32_getGw
    #define System_saveGw             Esp32_saveGw
    #define System_getMac             Esp32_getMac
    #define System_getCpu             Esp32_getCpu
    #define System_getRam             Esp32_getRam
    #define System_cpuReset           Esp32_cpuReset
    #define System_cpuResetReason     Esp32_cpuResetReason
    #define System_getRandom          Esp32_getRandom
    #define System_getRssi            Esp32_getRssi
    #define System_flashIsProtected   Esp32_flashIsProtected
    #define System_getCpuTemperature  Esp32_getCpuTemperature
    #define System_isWifiEnable       Esp32_isWifiEnable
    #define System_setWifiEnable      Esp32_SetWifiEnable

#else
    #error "board/hardware is not defined"
#endif

#endif /* _SYSTEM_H_ */

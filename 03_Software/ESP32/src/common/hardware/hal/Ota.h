#ifndef _OTA_H_
#define _OTA_H_

#ifdef ESP32
    #include "Esp32Ota.h"
    #define Ota_init                   Esp32Ota_init
    #define Ota_InitTask               Esp32Ota_InitTask
    #define Ota_inProgress             Esp32Ota_inProgress
    #define Ota_begin                  Esp32Ota_begin
    #define Ota_writeHexData           Esp32Ota_writeHexData
    #define Ota_end                    Esp32Ota_end
    #define Ota_getCurrentPartition    Esp32Ota_getCurrentPartition
    #define Ota_getCurrentBootAddr     Esp32Ota_getCurrentBootAddr
    #define Ota_getNextBootPartition   Esp32Ota_getNextBootPartition
    #define Ota_getAddrBootPartition   Esp32Ota_getAddrBootPartition
    #define Ota_getPartitionStruct     Esp32Ota_getPartitionStruct
#else
    #error "board/hardware is not defined"
#endif

#endif /* _OTA_H_ */

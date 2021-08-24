#ifndef _ACCESS_POINT_H_
#define _ACCESS_POINT_H_

#ifdef ESP32
    #include "Esp32AccessPoint.h"
    #define AccessPoint_init               Esp32AccessPoint_init
    #define AccessPoint_getPassword        Esp32AccessPoint_getPassword
    #define AccessPoint_getSSID            Esp32AccessPoint_getSSID
    #define AccessPoint_findSSIDAndPass    Esp32AccessPoint_findSSIDAndPass
#else
    #error "board/hardware is not defined"
#endif

#endif /* _ACCESS_POINT_H_ */


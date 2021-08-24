#ifndef ESP_WIFI_H_
#define ESP_WIFI_H_

#ifdef ESP32
    #include "Esp32Wifi.h"
    #define Wifi_init                    Esp32Wifi_init
    #define Wifi_saveSSIDAndPass         Esp32Wifi_saveSSIDAndPass
    #define Wifi_getSsid                 Esp32Wifi_getSSID
    #define Wifi_getPassword             Esp32Wifi_getPassword
    #define Wifi_removeSsidAndPass       Esp32Wifi_removeSsidAndPass
#else
    #error "board/hardware is not defined"
#endif

#endif /* ESP_WIFI_H_ */


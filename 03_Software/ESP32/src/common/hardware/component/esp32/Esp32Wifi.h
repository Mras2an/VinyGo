#ifndef ESP32_WIFI_H_
#define ESP32_WIFI_H_

void Esp32Wifi_init();

void Esp32Wifi_saveSSIDAndPass(const char * ssid, const char * password);

const char * Esp32Wifi_getSSID(void);

const char * Esp32Wifi_getPassword(void);

void Esp32Wifi_removeSsidAndPass(void);

#endif /* ESP32_WIFI_H_ */



#ifndef _ESP32_ACCESS_POINT_H_
#define _ESP32_ACCESS_POINT_H_

void Esp32AccessPoint_init(void);
const char * Esp32AccessPoint_getPassword(void);
const char * Esp32AccessPoint_getSSID(void);
void Esp32AccessPoint_findSSIDAndPass();

#endif /* _ESP32_ACCESS_POINT_H_ */


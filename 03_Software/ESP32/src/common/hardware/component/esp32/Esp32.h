#ifndef _ESP32_H_
#define _ESP32_H_
#include "esp_system.h"

void Esp32_init(void);

void Esp32_delete(void);

char * Esp32_getIp(void);

void Esp32_saveIp(const char * ip);

char * Esp32_getMask(void);

void Esp32_saveMask(const char * mask);

char * Esp32_getGw(void);

void Esp32_saveGw(const char * gw);

char * Esp32_getMac(void);

int Esp32_getCpu(void);

int Esp32_getRam(void);

void Esp32_cpuReset();

int Esp32_cpuResetReason(int reason);

uint32_t  Esp32_getRandom(void);

int Esp32_getRssi(void);

void Esp32_flashIsProtected(char * tmp);

float Esp32_getCpuTemperature(void);

int Esp32_isWifiEnable();

void Esp32_SetWifiEnable();

#endif /* _ESP32_H_ */


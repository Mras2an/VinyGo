#ifndef ESP32_OTA_H_
#define ESP32_OTA_H_

#include "esp_ota_ops.h"

typedef enum {
    OTA_OK = 0,
    OTA_ERR_PARTITION_NOT_FOUND = 1,
    OTA_ERR_PARTITION_NOT_ACTIVATED = 2,
    OTA_ERR_BEGIN_FAILED = 3,
    OTA_ERR_WRITE_FAILED = 4,
    OTA_ERR_END_FAILED = 5,
} TOtaResult;

void Esp32Ota_init();

void Esp32Ota_InitTask();

int Esp32Ota_inProgress();

TOtaResult Esp32Ota_begin();

TOtaResult Esp32Ota_writeHexData(const char *hexData, int len);

TOtaResult Esp32Ota_end();

char * Esp32Ota_getCurrentPartition();

unsigned int Esp32Ota_getCurrentBootAddr();

void Esp32Ota_getNextBootPartition(char * part);

const esp_partition_t * Esp32Ota_getPartitionStruct(int addrPart);

int Esp32Ota_getAddrBootPartition(char * part);
#endif /* ESP32_OTA_H_ */

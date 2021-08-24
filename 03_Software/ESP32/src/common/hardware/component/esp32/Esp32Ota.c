//License: Apache (https://github.com/classycodeoss/esp32-ota)

#ifdef ESP32
#include "esp_ota_ops.h"
#include "Esp32Ota.h"
#include "Debug.h"

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_event_loop.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/dns.h"

#include "Lcd.h"

static const esp_partition_t *sOtaPartition;

static uint32_t sFlashCurrentAddress;
static esp_ota_handle_t sOtaHandle;

static const esp_partition_t *Esp32Ota_findNextBootPartition();
static void Esp32Ota_networkTask(void *pvParam);
static void Esp32Ota_processMessage(const char *message, int messageLen, char *responseBuf, int responseBufLen);
static int Esp32Ota_networkReceive(int s, char *buf, int maxLen, int *actualLen);

#define PART_LEN 20
typedef struct sEsp32Ota {
    char currentPartition[PART_LEN];
    unsigned int addr;
} sEsp32Ota_t;

static sEsp32Ota_t sEsp32Ota;
#define this (&sEsp32Ota)


typedef enum {
    NWR_READ_COMPLETE = 0,
    NWR_READ_TIMEOUT = 1,
    NWR_DISCONNECTED = 2,
    NWR_ERROR = 3,
} TNetworkResult;

#define TAG "network"

#define NORM_C(c) (((c) >= 32 && (c) < 127) ? (c) : '.')

#define AP_CONNECTION_ESTABLISHED (1 << 0)
static EventGroupHandle_t sEventGroup;

static int sRebootAfterReply;

/*!*****************************************************************************
 * \fn         void Esp32Ota_init()
 * \param  none
 * \return     none
 *******************************************************************************/
void Esp32Ota_init()
{
    const esp_partition_t * currentBootPartition = esp_ota_get_boot_partition();
    memset(this->currentPartition, '\0', PART_LEN);
    memcpy(this->currentPartition, currentBootPartition->label, strlen(currentBootPartition->label));
    this->addr = currentBootPartition->address;
    Debug_info("Part: %s, addr:%08x \n", Esp32Ota_getCurrentPartition(), Esp32Ota_getCurrentBootAddr());
}

/*!*****************************************************************************
 * \fn         char * Esp32Ota_getCurrentPartition()
 * \param  none
 * \return     char *
 *******************************************************************************/
char * Esp32Ota_getCurrentPartition()
{
    return this->currentPartition;
}

/*!*****************************************************************************
 * \fn         unsigned int Esp32Ota_getCurrentAddr()
 * \param  none
 * \return     unsigned int
 *******************************************************************************/
unsigned int Esp32Ota_getCurrentBootAddr()
{
    return this->addr;
}
/*!*****************************************************************************
 * \fn         int Esp32Ota_inProgress()
 * \param  none
 * \return     int
 *******************************************************************************/
int Esp32Ota_inProgress()
{
    return sOtaPartition ? 1 : 0;
}

/*!*****************************************************************************
 * \fn         TOtaResult Esp32Ota_begin()
 * \param  none
 * \return     TOtaResult
 *******************************************************************************/
TOtaResult Esp32Ota_begin()
{
    //Lcd_sendOtaInfo(strlen("in progress..."), "in progress...");
    sOtaPartition = Esp32Ota_findNextBootPartition();

    if(!sOtaPartition) {
        return OTA_ERR_PARTITION_NOT_FOUND;
    }

    sFlashCurrentAddress = sOtaPartition->address;
    Debug_info("Set start address for flash writes to 0x%08x\n", sFlashCurrentAddress);
    esp_err_t result = esp_ota_begin(sOtaPartition, HW_BLOCK_SIZE, &sOtaHandle);
    Debug_info("Result from esp_ota_begin: %d %d\n", result, sOtaHandle);

    if(result == ESP_OK) {
        return OTA_OK;
    }

    return OTA_ERR_BEGIN_FAILED;
}

/*!*****************************************************************************
 * \fn         TOtaResult Esp32Ota_end()
 * \param  none
 * \return     TOtaResult
 *******************************************************************************/
TOtaResult Esp32Ota_end()
{
    //Lcd_sendOtaInfo(strlen("OTA is finish"), "OTA is finish");
    if(!sOtaPartition) {
        return OTA_ERR_PARTITION_NOT_FOUND;
    }

    esp_err_t result = esp_ota_end(sOtaHandle);

    if(result != ESP_OK) {
        return OTA_ERR_END_FAILED;
    }

    result = esp_ota_set_boot_partition(sOtaPartition);

    if(result == ESP_OK) {
        Debug_info("Boot partition activated: %s\n", sOtaPartition->label);
        return OTA_OK;
    }

    Debug_err("Failed to activate boot partition %s, error %d\n", sOtaPartition->label, result);
    sOtaPartition = NULL;
    return OTA_ERR_PARTITION_NOT_ACTIVATED;
}

/*!*****************************************************************************
 * \fn         TOtaResult Esp32Ota_writeHexData(const char *hexData, int len)
 * \param  int
 * \param  const char *
 * \return     TOtaResult
 *******************************************************************************/
TOtaResult Esp32Ota_writeHexData(const char *hexData, int len)
{
    uint8_t buf[HW_BLOCK_SIZE];

    for(int i = 0; i < HW_BLOCK_SIZE; i++) {
        buf[i] = (i < len) ? hexData[i] : 0xff;
    }

    if(sFlashCurrentAddress % 0x1000 == 0) {
        int flashSectorToErase = sFlashCurrentAddress / 0x1000;
        Debug_info("Erasing flash sector %d\n", flashSectorToErase);
        spi_flash_erase_sector(flashSectorToErase);
    }

    Debug_info("Writing flash at 0x%08x...\n", sFlashCurrentAddress);
    esp_err_t result = esp_ota_write(sOtaHandle, buf, len);

    if(result != ESP_OK) {
        Debug_err("Failed to write flash at address 0x%08x, error %d\n", sFlashCurrentAddress, result);
        return OTA_ERR_WRITE_FAILED;
    }

    sFlashCurrentAddress += len;
    return OTA_OK;
}

/*!*****************************************************************************
 * \fn         static const esp_partition_t *Esp32Ota_findNextBootPartition()
 * \param  none
 * \return     const esp_partition_t *
 *******************************************************************************/
static const esp_partition_t *Esp32Ota_findNextBootPartition()
{
    const esp_partition_t *currentBootPartition = esp_ota_get_boot_partition();
    const esp_partition_t *nextBootPartition = NULL;

    if(!strcmp("factory", currentBootPartition->label)) {
        nextBootPartition = esp_partition_find_first(ESP_PARTITION_TYPE_APP, ESP_PARTITION_SUBTYPE_ANY, "ota_0");
    }

    if(!strcmp("ota_0", currentBootPartition->label)) {
        nextBootPartition = esp_partition_find_first(ESP_PARTITION_TYPE_APP, ESP_PARTITION_SUBTYPE_ANY, "ota_1");
    }

    if(!strcmp("ota_1", currentBootPartition->label)) {
        nextBootPartition = esp_partition_find_first(ESP_PARTITION_TYPE_APP, ESP_PARTITION_SUBTYPE_ANY, "ota_0");
    }

    if(nextBootPartition) {
        Debug_info("Found next boot partition: %02x %02x 0x%08x %s",
                   nextBootPartition->type, nextBootPartition->subtype, nextBootPartition->address, nextBootPartition->label);
    } else {
        Debug_err("Failed to determine next boot partition from current boot partition: %s",
                  currentBootPartition ? currentBootPartition->label : "NULL");
    }

    return nextBootPartition;
}

/*!*****************************************************************************
 * \fn         void Esp32Ota_getNextBootPartition(char * part)
 * \param  char *
 * \return     void
 *******************************************************************************/
void Esp32Ota_getNextBootPartition(char * part)
{
    const esp_partition_t *currentBootPartition = esp_ota_get_boot_partition();

    if(!strcmp("factory", currentBootPartition->label)) {
        memcpy(part, "ota_0", strlen("ota_0"));
    }

    if(!strcmp("ota_0", currentBootPartition->label)) {
        memcpy(part, "ota_1", strlen("ota_1"));
    }

    if(!strcmp("ota_1", currentBootPartition->label)) {
        memcpy(part, "ota_0", strlen("ota_0"));
    }
}

/*!*****************************************************************************
 * \fn         const esp_partition_t * Esp32Ota_getPartitionStruct(int addrPart)
 * \param  const esp_partition_t *
 * \return     void
 *******************************************************************************/
const esp_partition_t * Esp32Ota_getPartitionStruct(int addrPart)
{
    if(ADDR_PART_1 == addrPart) {
        return esp_ota_get_boot_partition();
    }

    if(ADDR_PART_2 == addrPart) {
        return esp_partition_find_first(ESP_PARTITION_TYPE_APP, ESP_PARTITION_SUBTYPE_ANY, "ota_0");
    }

    if(ADDR_PART_3 == addrPart) {
        return esp_partition_find_first(ESP_PARTITION_TYPE_APP, ESP_PARTITION_SUBTYPE_ANY, "ota_1");
    }

    return NULL;
}

/*!*****************************************************************************
 * \fn         int Esp32Ota_getAddrBootPartition(char * part)
 * \param  none
 * \return     char *
 *******************************************************************************/
int Esp32Ota_getAddrBootPartition(char * part)
{
    if(!strcmp("factory", part)) {
        return ADDR_PART_1;
    }

    if(!strcmp("ota_0", part)) {
        return ADDR_PART_2;
    }

    if(!strcmp("ota_1", part)) {
        return ADDR_PART_3;
    }

    return 0;
}

/******************************************************************************
 * \fn : void Esp32Ota_InitTask()
 * \param   : void
 * \return      : void
 *******************************************************************************/
void Esp32Ota_InitTask()
{
    Debug_info("InitTask \n");
    sEventGroup = xEventGroupCreate();
    xTaskCreate(&Esp32Ota_networkTask, "networkTask", 32768, NULL, 5, NULL);
}

/******************************************************************************
 * \fn : static int Esp32Ota_networkReceive(int s, char *buf, int maxLen, int *actualLen)
 * \param   : int
 * \param   : char *
 * \param   : int
 * \param   : int
 * \return      : int
 *******************************************************************************/
static int Esp32Ota_networkReceive(int s, char *buf, int maxLen, int *actualLen)
{
    int totalLen = 0;

    for(int timeoutCtr = 0; timeoutCtr < 3000; timeoutCtr++) {
        int readAgain = 0;

        do {
            buf[totalLen] = 0x00;
            int n = recv(s, &buf[totalLen], maxLen - totalLen, MSG_DONTWAIT);
            int e = errno;

            if(n > 0) {
                totalLen += n;

                if(totalLen > 0) {
                    int recordLen = 0;
                    int recordWithLengthIndicator = (1 == sscanf(buf, "!%04x", &recordLen));
                    Debug_info("networkReceive: recordWithLengthIndicator = %d, expected length = %d, current length = %d\n", recordWithLengthIndicator, recordLen, totalLen);

                    if((recordWithLengthIndicator && totalLen == recordLen)
                       || (!recordWithLengthIndicator && buf[totalLen - 1] == '\n')) {
                        Debug_info("networkReceive: received %d byte packet on socket %d\n", totalLen, s);
                        *actualLen = totalLen;
                        return NWR_READ_COMPLETE;
                    }
                }

                readAgain = 1;
            } else if(n < 0 && e == EAGAIN) {
                readAgain = 0;
            } else {
                Debug_err("recv n = %d, errno = %d (%s)\n", n, e, strerror(e));
                return NWR_ERROR;
            }
        } while(readAgain);

        vTaskDelay(10 / portTICK_RATE_MS);
    }

    return NWR_READ_TIMEOUT;
}

/******************************************************************************
 * \fn : static void Esp32Ota_networkTask(void *pvParam)
 * \param   : void *
 * \return      : void
 *******************************************************************************/
static void Esp32Ota_networkTask(void *pvParam)
{
    const int maxRequestLen = 10000;
    const int maxResponseLen = 1000;
    const int tcpPort = 80;
    Debug_info("OTA Task\n");

    while(1) {
        int s = socket(AF_INET, SOCK_STREAM, 0);

        if(s < 0) {
            Debug_err("networkTask: failed to create socket: %d (%s)\n", errno, strerror(errno));
            vTaskDelay(1000 / portTICK_RATE_MS);
            continue;
        }

        struct sockaddr_in serverAddr;

        memset(&serverAddr, 0, sizeof(struct sockaddr_in));

        serverAddr.sin_len = sizeof(struct sockaddr_in);

        serverAddr.sin_family = AF_INET;

        serverAddr.sin_port = htons(tcpPort);

        serverAddr.sin_addr.s_addr = INADDR_ANY;

        int b = bind(s, (struct sockaddr *)&serverAddr, sizeof(struct sockaddr_in));

        if(b < 0) {
            Debug_err("networkTask: failed to bind socket %d: %d (%s)\n", s, errno, strerror(errno));
            vTaskDelay(1000 / portTICK_RATE_MS);
            continue;
        }

        Debug_info("networkTask: 'listen' on socket %d\n", s);
        listen(s, 1);

        while(1) {
            Debug_info("--------------------\n");
            Debug_info("networkTask: 'accept' on socket %d\n", s);
            struct sockaddr_in clientAddr;
            socklen_t clen = sizeof(clientAddr);
            int s2 = accept(s, (struct sockaddr *)&clientAddr, &clen);

            if(s2 < 0) {
                Debug_err("networkTask: 'accept' failed: %d (%s)\n", errno, strerror(errno));
                vTaskDelay(1000 / portTICK_RATE_MS);
                break;
            }

            do {
                char *requestBuf = malloc(maxRequestLen * sizeof(char));

                if(!requestBuf) {
                    Debug_err("networkTask: malloc for requestBuf failed: %d (%s)\n", errno, strerror(errno));
                    break;
                }

                bzero(requestBuf, maxRequestLen);
                int totalRequestLen = 0;
                TNetworkResult result = Esp32Ota_networkReceive(s2, requestBuf, maxRequestLen, &totalRequestLen);

                if(result != NWR_READ_COMPLETE) {
                    Debug_info("nothing more to, closing socket %d\n", s2);
                    free(requestBuf);
                    close(s2);
                    break;
                }

                Debug_info("networkTask: received %d bytes: %02x %02x %02x %02x ... | %c%c%c%c...\n",
                           totalRequestLen,
                           requestBuf[0], requestBuf[1], requestBuf[2], requestBuf[3],
                           NORM_C(requestBuf[0]), NORM_C(requestBuf[1]), NORM_C(requestBuf[2]), NORM_C(requestBuf[3]));
                char *responseBuf = malloc(maxResponseLen * sizeof(char));
                Esp32Ota_processMessage(requestBuf, totalRequestLen, responseBuf, maxResponseLen);
                free(requestBuf);
                int totalLen = strlen(responseBuf);
                int nofWritten = 0;
                Debug_info("networkTask: write %d bytes to socket %d: %02x %02x %02x %02x ... | %c%c%c%c...\n", totalLen, s2,
                           responseBuf[0], responseBuf[1], responseBuf[2], responseBuf[3],
                           NORM_C(responseBuf[0]), NORM_C(responseBuf[1]), NORM_C(responseBuf[2]), NORM_C(responseBuf[3]));

                do {
                    int n = write(s2, &responseBuf[nofWritten], totalLen - nofWritten);
                    int e = errno;

                    if(n > 0) {
                        nofWritten += n;

                        if(totalLen - nofWritten == 0) {
                            break;
                        }
                    } else if(n == 0) {
                        // Disconnected?
                        break;
                    } else {
                        if(e == EAGAIN) {
                            continue;
                        }

                        Debug_err("networkTask: write failed: %d (%s)\n", errno, strerror(errno));
                        break;
                    }
                } while(1);

                free(responseBuf);

                if(sRebootAfterReply) {
                    Debug_info("networkTask: Reboot in 2 seconds...\n");
                    vTaskDelay(2000 / portTICK_RATE_MS);
                    esp_restart();
                }
            } while(1);
        }

        close(s);
        vTaskDelay(2000 / portTICK_RATE_MS);
    }
}

/******************************************************************************
 * \fn : static void Esp32Ota_processMessage(const char *message, int messageLen, char *responseBuf, int responseBufLen)
 * \param   : const char *
 * \param   : int
 * \param   : char *
 * \param   : int
 * \return      : void
 *******************************************************************************/
static void Esp32Ota_processMessage(const char *message, int messageLen, char *responseBuf, int responseBufLen)
{
    char response[256];
    sprintf(response, "OK\r\n");

    if(message[0] == '!') {
        TOtaResult result = OTA_OK;

        if(message[1] == '[') {
            Debug_info("processMessage: OTA start\n");
            Lcd_sendOtaInfo(strlen("In progress..."), "in progress...");
            result = Esp32Ota_begin();
        } else if(message[1] == ']') {
            Debug_info("processMessage: OTA end\n");
            Lcd_sendOtaInfo(strlen("End of OTA, system reboot"), "End of OTA, system reboot");
            result = Esp32Ota_end();
        } else if(message[1] == '*') {
            Debug_info("processMessage: Reboot\n");
            sRebootAfterReply = 1;
        } else {
            result = Esp32Ota_writeHexData(&message[5], messageLen - 5);
        }

        if(result != OTA_OK) {
            Debug_err("processMessage: OTA_ERROR %d\n", result);
            sprintf(response, "OTA_ERROR %d\r\n\n", result);
        }
    }

    strncpy(responseBuf, response, responseBufLen);
}

#endif // ESP32

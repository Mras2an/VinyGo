
#ifdef ESP32
#include "esp_wifi.h"
#include "Bsp.h"
#include "Esp32AccessPoint.h"
#include "System.h"

typedef struct sAccessPoint {
    char * ssid;
    char * pass;
} sAccessPoint_t;

static sAccessPoint_t sAccessPoint;
#define this (&sAccessPoint)

static wifi_config_t Esp32AccessPoint_getSSIDAndPassStruct(void);

/******************************************************************************
 * FunctionName : void Esp32AccessPoint_init(void)
 * Parameters   : wifi_config_t
 * Returns      : void
 *******************************************************************************/
void Esp32AccessPoint_init(void)
{
    wifi_config_t wifi_config = Esp32AccessPoint_getSSIDAndPassStruct();

    if((wifi_config.ap.ssid[0] == '\0') || (wifi_config.ap.password[0] == '\0')) {
        return;
    }

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
    Debug_info("Setting Wi-fi AP SSID: %s\n", wifi_config.ap.ssid);
    Debug_info("Setting Wi-fi AP PASS: %s\n", wifi_config.ap.password);
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());
    System_setWifiEnable();
}

/******************************************************************************
 * FunctionName : static void Esp32AccessPoint_getSSIDAndPassStruct(const char * ssid, const char * password)
 * Parameters   : const char *
 * Parameters   : const char *
 * Returns      : void
 *******************************************************************************/
static wifi_config_t Esp32AccessPoint_getSSIDAndPassStruct(void)
{
    wifi_config_t wifi_config;
    wifi_config.ap.ssid_len = 0;
    wifi_config.ap.authmode = WIFI_AUTH_WPA2_PSK;
    wifi_config.ap.beacon_interval = 400;
    wifi_config.ap.max_connection = 4;
    wifi_config.ap.ssid_hidden = 0;
    char * ssid = (char *)Esp32AccessPoint_getSSID();
    char * password = (char *)Esp32AccessPoint_getPassword();
    memset(wifi_config.ap.ssid, '\0', 32);
    memset(wifi_config.ap.password, '\0', 64);

    if(ssid != NULL) {
        memcpy(wifi_config.ap.ssid, ssid, strlen(ssid));
        OsFree(ssid);
        ssid = NULL;
    }

    if(password != NULL) {
        memcpy(wifi_config.ap.password, password, strlen(password));
        OsFree(password);
        password = NULL;
    }

    return wifi_config;
}

/******************************************************************************
 * FunctionName : void Esp32AccessPoint_findSSIDAndPass()
 * Parameters   : void
 * Returns      : void
 *******************************************************************************/
void Esp32AccessPoint_findSSIDAndPass()
{
    this->ssid = NULL;
    this->ssid = OsMalloc(32 + 1);
    memset(this->ssid, '\0', 32 + 1);
    this->pass = NULL;
    this->pass = OsMalloc(64 + 1);
    memset(this->pass, '\0', 64 + 1);
    memcpy(this->ssid, "HRC", 3);
    memcpy(this->pass, "HRC2yBqT", 8);
}

/******************************************************************************
 * FunctionName : const char * Esp32AccessPoint_getSSID(void)
 * Parameters   : void
 * Returns      : const char *
 *******************************************************************************/
const char * Esp32AccessPoint_getSSID(void)
{
    return this->ssid;
}

/******************************************************************************
 * FunctionName : const char * Esp32AccessPoint_getPassword(void)
 * Parameters   : void
 * Returns      : const char *
 *******************************************************************************/
const char * Esp32AccessPoint_getPassword(void)
{
    return this->pass;
}
#endif // ESP32

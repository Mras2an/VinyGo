#ifdef ESP32
#include "esp_wifi.h"
#include "esp_event_loop.h"

#include "rom/rtc.h"
#include "Board.h"
#include "System.h"
#include "esp_flash_encrypt.h"
#include "FileSystem.h"
#include "Ota.h"
#include "mdns.h"

#include "Lcd.h"

OsTimerHandle_t esp32Timer;
#define  WIFI_PERIOD      (20000 / portTICK_PERIOD_MS)
#define  MD5_LEN_TO_READ  1040384

static esp_err_t Esp32_eventHandler(void *ctx, system_event_t *event);
static void Esp32_Callback(OsTimerHandle_t esp32Timer);

static void Esp32_Callback(OsTimerHandle_t esp32Timer)
{
    System_cpuReset();
}

typedef struct sEsp32 {
    char * ip;
    char * mask;
    char * gw;
    char * mac;
    char * uniqueId;
    char * md5Read;
    char * md5Cal;
    int enable;
} sEsp32_t;

static sEsp32_t sEsp32;
#define this (&sEsp32)

extern uint8_t temprature_sens_read();

/******************************************************************************
 * \fn : float Esp32_getCpuTemperature(void)
 * \param   : void
 * \return      : float
 *******************************************************************************/
float Esp32_getCpuTemperature(void)
{
    return (float)((temprature_sens_read() - 32) / 1.8);
}

/******************************************************************************
 * \fn : static esp_err_t Esp32_eventHandler(void *ctx, system_event_t *event)
 * \param   : void *, system_event_t *
 * \return      : esp_err_t
 *******************************************************************************/
static esp_err_t Esp32_eventHandler(void *ctx, system_event_t *event)
{
    switch(event->event_id) {
        case SYSTEM_EVENT_STA_START: {
            Debug_info("started\n");
            esp_wifi_connect();
            esp32Timer = OsTimerCreate("Timer", WIFI_PERIOD, pdTRUE, 0, Esp32_Callback);

            if(esp32Timer == NULL) {
                Debug_err("Create timer\n");
            }

            if(OsTimerReset(esp32Timer, 0) != pdPASS) {
                Debug_err("Reset timer\n");
            }
        }
        break;

        case SYSTEM_EVENT_STA_DISCONNECTED: {
            Debug_info("disconnected\n");
        }
        break;

        case SYSTEM_EVENT_SCAN_DONE: {
            Debug_info("Scan done\n");
        }
        break;

        case SYSTEM_EVENT_STA_CONNECTED: {
            Debug_info("connected\n");
        }
        break;

        case SYSTEM_EVENT_STA_GOT_IP: {
            Debug_info("dhcp => event_handler:SYSTEM_EVENT_STA_GOT_IP!\n");
            Esp32_saveIp((const char *)ip4addr_ntoa(&event->event_info.got_ip.ip_info.ip));
            Esp32_saveMask((const char *)ip4addr_ntoa(&event->event_info.got_ip.ip_info.netmask));
            Esp32_saveGw((const char *)ip4addr_ntoa(&event->event_info.got_ip.ip_info.gw));
            Esp32_getMac();
        }
        break;

        case SYSTEM_EVENT_AP_STACONNECTED: {
            Debug_info("station:"MACSTR" join,AID=%d\n", MAC2STR(event->event_info.sta_connected.mac), event->event_info.sta_connected.aid);
Ota_InitTask();
Lcd_sendOtaInfo(strlen("Wifi connection detected"), "Wifi connection detected");
}
break;

    case SYSTEM_EVENT_AP_STADISCONNECTED:
{
Debug_info("station:"MACSTR"leave,AID=%d\n", MAC2STR(event->event_info.sta_disconnected.mac), event->event_info.sta_disconnected.aid);
}
break;

    case SYSTEM_EVENT_STA_STOP:
{
Debug_info("stopped\n");
}
break;

    default:
{
Debug_info("Unhandled event (%d)\n", event->event_id);
}
break;
  }
  return ESP_OK;
}



/******************************************************************************
 * \fn : uint32_t  Esp32_getRandom(void)
 * \param   : void
 * \return      : uint32_t
 *******************************************************************************/
uint32_t Esp32_getRandom(void)
{
  return esp_random();
}

/******************************************************************************
 * \fn : void Esp32_getRssi(void)
 * \param   : void
 * \return      : int
 *******************************************************************************/
int Esp32_getRssi(void)
{
  wifi_ap_record_t wifidata;
  if (esp_wifi_sta_get_ap_info(&wifidata) == 0)
  {
    return wifidata.rssi;
  }
  return 0;
}

/******************************************************************************
 * \fn : char * Esp32_getMac(void)
 * \param   : void
 * \return      : char *
 *******************************************************************************/
char * Esp32_getMac(void)
{
  uint8_t data[6];
  esp_wifi_get_mac(WIFI_IF_STA, data);
  sprintf(this->mac, "%02x:%02x:%02x:%02x:%02x:%02x", data[0], data[1], data[2], data[3], data[4], data[5]);

  return this->mac;
}

/******************************************************************************
 * \fn : char * Esp32_getIp(void)
 * \param   : void
 * \return      : char *
 *******************************************************************************/
char * Esp32_getIp(void)
{
  return this->ip;
}

/******************************************************************************
 * \fn : void Esp32_saveIp(char * ip)
 * \param   : char *
 * \return      : void
 *******************************************************************************/
void Esp32_saveIp(const char * ip)
{
  size_t ipLen = strlen(ip) + 1;
  this->ip = OsMalloc(ipLen);
  memset(this->ip, '\0', ipLen);
  memcpy(this->ip, ip, ipLen);
  Debug_info("%s\n", this->ip);
}

/******************************************************************************
 * \fn : char * Esp32_getGw(void)
 * \param   : void
 * \return      : char *
 *******************************************************************************/
char * Esp32_getGw(void)
{
  return this->gw;
}

/******************************************************************************
 * \fn : void Esp32_saveGw(char * ip)
 * \param   : char *
 * \return      : void
 *******************************************************************************/
void Esp32_saveGw(const char * gw)
{
  size_t gwLen = strlen(gw) + 1;
  this->gw = OsMalloc(gwLen);
  memset(this->gw, '\0', gwLen);
  memcpy(this->gw, gw, gwLen);
}

/******************************************************************************
 * \fn : char * Esp32_getMask(void)
 * \param   : void
 * \return      : char *
 *******************************************************************************/
char * Esp32_getMask(void)
{
  return this->mask;
}

/******************************************************************************
 * \fn : void Esp32_saveMask(char * ip)
 * \param   : char *
 * \return      : void
 *******************************************************************************/
void Esp32_saveMask(const char * mask)
{
  size_t maskLen = strlen(mask) + 1;
  this->mask = OsMalloc(maskLen);
  memset(this->mask, '\0', maskLen);
  memcpy(this->mask, mask, maskLen);
}

/******************************************************************************
 * \fn : int Esp32_isWifiEnable()
 * \param   : void
 * \return      : int
 *******************************************************************************/
int Esp32_isWifiEnable()
{
	return this->enable;
}

/******************************************************************************
 * \fn : void Esp32_SetWifiEnable()
 * \param   : void
 * \return      : void
 *******************************************************************************/
void Esp32_SetWifiEnable()
{
	this->enable = 1;
}
/******************************************************************************
 * \fn : void Esp32_init(void)
 * \param   : void
 * \return      : void
 *******************************************************************************/
void Esp32_init(void)
{
	this->enable = 0;
  this->ip = NULL;
  this->gw = NULL;
  this->mask = NULL;
  this->mac = OsMalloc(sizeof(char) * 17);
  this->uniqueId = NULL;
  this->uniqueId = OsMalloc(sizeof(char) * 16);
  this->md5Read = NULL;
  this->md5Read = OsMalloc(sizeof(char) * 33);
  this->md5Cal = NULL;
  this->md5Cal = OsMalloc(sizeof(char) * 33);

  tcpip_adapter_init();

  /* Set ip static */
  tcpip_adapter_ip_info_t info = { 0,};
  IP4_ADDR(&info.ip, 192, 168, 52, 1);
  IP4_ADDR(&info.gw, 192, 168, 52, 1);
  IP4_ADDR(&info.netmask, 255, 255, 255, 0);
  ESP_ERROR_CHECK(tcpip_adapter_dhcps_stop(TCPIP_ADAPTER_IF_AP));
  ESP_ERROR_CHECK(tcpip_adapter_set_ip_info(TCPIP_ADAPTER_IF_AP, &info));
  ESP_ERROR_CHECK(tcpip_adapter_dhcps_start(TCPIP_ADAPTER_IF_AP));

  Debug_info("Free Heap at %d = %d\n", __LINE__, OsGetFreeHeapSize());
  ESP_ERROR_CHECK(esp_event_loop_init(Esp32_eventHandler, NULL));

}

/******************************************************************************
 * \fn : int Esp32_getCpu(void)
 * \param   : void
 * \return      : int
 *******************************************************************************/
int Esp32_getCpu(void)
{
  return CONFIG_ESP32_DEFAULT_CPU_FREQ_MHZ;
}

/******************************************************************************
 * \fn : int Esp32_getRam(void)
 * \param   : void
 * \return      : int
 *******************************************************************************/
int Esp32_getRam(void)
{
  return OsGetFreeHeapSize();
}

/******************************************************************************
 * \fn : void Esp32_cpuReset()
 * \param   : void
 * \return      : void
 *******************************************************************************/
void Esp32_cpuReset()
{
  Debug_err("\n");
  esp_restart();
}

/******************************************************************************
 * \fn : int Esp32_cpuResetReason(int reason)
 * \param   : int
 * \return      : int
 *******************************************************************************/
int Esp32_cpuResetReason(int reason)
{
  return rtc_get_reset_reason(reason);
}

/******************************************************************************
 * \fn : void Esp32_flashIsProtected(char * tmp)
 * \param   : char *
 * \return      : void
 *******************************************************************************/
void Esp32_flashIsProtected(char * tmp)
{
  if (esp_flash_encryption_enabled())
  {
    memcpy(tmp, "true", 4);
  }
  else
  {
    memcpy(tmp, "false", 5);
  }
}
/******************************************************************************
 * \fn : void Esp32_delete(void)
 * \param   : void
 * \return      : void
 *******************************************************************************/
void Esp32_delete(void)
{
  if(this->ip)
  {
    OsFree(this->ip);
  }
  if(this->gw)
  {
    OsFree(this->gw);
  }
  if(this->mask)
  {
    OsFree(this->mask);
  }
  if(this->mac)
  {
    OsFree(this->mac);
  }
  if(this->uniqueId)
  {
    OsFree(this->uniqueId);
  }
  if(this->md5Read)
  {
    OsFree(this->md5Read);
  }
  if(this->md5Cal)
  {
    OsFree(this->md5Cal);
  }
}
#endif // ESP32

#ifndef _BSP_H_
#define _BSP_H_

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "Debug.h"

#ifdef FREERTOS_V9
    #include "freertos/FreeRTOS.h"
    #include "freertos/queue.h"
    #include "freertos/task.h"
    #include "freertos/timers.h"
    #include "freertos/event_groups.h"
    #include "freertos/semphr.h"
    #include "esp_log.h"

    /* OS Allocation */
    #define OsMalloc(x)              pvPortMalloc(x)
    #define OsFree(x)                vPortFree(x)
    #define OsGetFreeHeapSize        xPortGetFreeHeapSize

    /* OS Timer */
    #define OsTimerStop              xTimerStop
    #define OsTimerReset             xTimerReset
    #define OsTimerCreate            xTimerCreate

    #define OsTimerHandle_t          TimerHandle_t

    /* OS Queue */
    #define OsQueueCreate            xQueueCreate
    #define OsQueueDelete            vQueueDelete
    #define OsQueueSend              xQueueSend
    #define OsQueueSendFromISR       xQueueSendFromISR
    #define OsQueueReceive           xQueueReceive

    #define OsQueueHandle_t          QueueHandle_t

    /* OS Task */
    #define OsTaskCreate             xTaskCreate
    #define OsTaskDelete             vTaskDelete

    #define OsTaskHandle_t           TaskHandle_t

    /* OS Event */
    #define OsEventGroupSetBits      xEventGroupSetBits
    #define OsEventGroupClearBits    xEventGroupClearBits
    #define OsEventGroupWaitBits     xEventGroupWaitBits

    #define OsEventGroupHandle_t     EventGroupHandle_t

    /* Semaphore / Mutexes */
    #define OsxSemaphoreTake         xSemaphoreTake
    #define OsxSemaphoreGive         xSemaphoreGive
    #define OsxSemaphoreCreateMutex  xSemaphoreCreateMutex

    #define OsSemaphoreHandle_t      SemaphoreHandle_t

    /* Timing Info */
    #define OsPortTimingPeriod       portMAX_DELAY

    /* Print Functions */
    #define OsPrintf                 printf

    #ifdef ESP32
        #define OsDebug_err(msg, ...)  ESP_LOGE(__func__, "[ERR : %s, L%d] "msg, __FUNCTION__, __LINE__, ##__VA_ARGS__)
    #else
        #define OsDebug_err(msg, ...)  printf("[ERR : %s, L%d] "msg, __FUNCTION__, __LINE__, ##__VA_ARGS__)
    #endif


#else
    #error "OS is not defined"
#endif

#endif // _BSP_H_

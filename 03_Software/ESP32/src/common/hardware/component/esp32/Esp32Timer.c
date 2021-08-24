#ifdef ESP32
#include "Esp32Timer.h"
#include "Debug.h"

/* TIMER */
#define TIMER_INTR_SEL        TIMER_INTR_LEVEL
#define TIMER_GROUP           TIMER_GROUP_0
#define TIMER_DIVIDER         16
#define TIMER_SCALE           (TIMER_BASE_CLK / TIMER_DIVIDER)
#define TIMER_FINE_ADJ        (1.4 * (TIMER_BASE_CLK / TIMER_DIVIDER)/1000000)
#define TEST_WITHOUT_RELOAD   0
#define TEST_WITH_RELOAD      1

typedef struct sTimer {
    pfctTimerCallback_t timerCallback;
} sTimer_t;
static sTimer_t sTimer;
#define this (&sTimer)

/******************************************************************************
 * \fn       void Esp32Timer_initItTimer(pfctTimerCallback_t timerCallback)
 * \brief    Type definition of function that initializes IT timer.
 * \param    timerCallback this is the callback for IT timer
 * \return   none
 *******************************************************************************/
void Esp32Timer_initItTimer(pfctTimerCallback_t timerCallback)
{
    if(timerCallback == NULL) {
        Debug_err("Need a timer callback.\n");
        return;
    }

    Debug_info("Timer init.\n");
    this->timerCallback = timerCallback;
    int timer_group = TIMER_GROUP_0;
    int timer_idx = TIMER_0;
    timer_config_t config;
    config.alarm_en = 1;
    config.auto_reload = 0;
    config.counter_dir = TIMER_COUNT_UP;
    config.divider = TIMER_DIVIDER;
    config.intr_type = TIMER_INTR_SEL;
    config.counter_en = TIMER_PAUSE;
    timer_init(timer_group, timer_idx, &config);
    timer_pause(timer_group, timer_idx);
    timer_enable_intr(timer_group, timer_idx);
    timer_isr_register(timer_group, timer_idx, timer_group0_isr, (void*) timer_idx, ESP_INTR_FLAG_IRAM, NULL);
    timer_set_counter_value(timer_group, timer_idx, 0x00);
    timer_set_alarm_value(timer_group, timer_idx, 0.00135 * TIMER_SCALE - TIMER_FINE_ADJ);
    timer_start(timer_group, timer_idx);
}

/******************************************************************************
 * \fn       void Esp32Timer_stopItTimer(void)
 * \brief    Type definition of function that stop IT timer.
 * \param    none
 * \return   none
 *******************************************************************************/
void Esp32Timer_stopItTimer(void)
{
    int timer_group = TIMER_GROUP_0;
    int timer_idx = TIMER_0;
    timer_pause(timer_group, timer_idx);
}

/******************************************************************************
 * \fn       void IRAM_ATTR timer_group0_isr(void *para)
 * \brief    Type definition of IT timer.
 * \param    none
 * \return   none
 *******************************************************************************/
void IRAM_ATTR timer_group0_isr(void *para)
{
    esp_intr_noniram_disable();
    int timer_idx = (int) para;
    uint32_t intr_status = TIMERG0.int_st_timers.val;

    if((intr_status & BIT(timer_idx)) && timer_idx == TIMER_0) {
        this->timerCallback();
    }

    esp_intr_noniram_enable();
}

/******************************************************************************
 * \fn       void Esp32Timer_reloadItTimer(double timeLoad)
 * \brief    Type definition of function that reload timer.
 * \param    timeLoad  The time in uS.
 * \return   none
 *******************************************************************************/
void Esp32Timer_reloadItTimer(double timeLoad)
{
    int timer_idx = TIMER_0;
    int timer_group = TIMER_GROUP_0;
    TIMERG0.hw_timer[timer_idx].update = 1;
    TIMERG0.int_clr_timers.t0 = 1;
    uint64_t timer_val = ((uint64_t) TIMERG0.hw_timer[timer_idx].cnt_high) << 32
                         | TIMERG0.hw_timer[timer_idx].cnt_low;
    timer_val +=
        (uint64_t)(timeLoad * (TIMER_BASE_CLK / TIMERG0.hw_timer[timer_idx].config.divider));
    timer_val -= TIMER_FINE_ADJ;
    TIMERG0.hw_timer[timer_idx].alarm_high = (uint32_t)(timer_val >> 32);
    TIMERG0.hw_timer[timer_idx].alarm_low = (uint32_t) timer_val;
    TIMERG0.hw_timer[timer_idx].config.alarm_en = 1;
    timer_start(timer_group, timer_idx);
}

#endif // ESP32

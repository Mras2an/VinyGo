#ifndef _ESP32_TIMER_H_
#define _ESP32_TIMER_H_

#include <string.h>
#include "driver/timer.h"

typedef void (*pfctTimerCallback_t)(void);

/**  @ingroup  GrpEsp32Hard
    \brief    Declaration of function that initializes IT timer.
*/
void Esp32Timer_initItTimer(pfctTimerCallback_t timerCallback);

/**  @ingroup  GrpEsp32Hard
    \brief    Declaration of function that stop IT timer.
*/
void Esp32Timer_stopItTimer(void);

/**  @ingroup  GrpEsp32Hard
    \brief    Declaration of function that reaload IT timer.
*/
void Esp32Timer_reloadItTimer(double timeLoad);

/**  @ingroup  GrpEsp32Hard
    \brief    Declaration of function that IT timer.
*/
void IRAM_ATTR timer_group0_isr(void * para);

#endif // _ESP32_TIMER_H_

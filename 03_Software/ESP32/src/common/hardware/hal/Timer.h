#ifndef _TIMER_H_
#define _TIMER_H_

#ifdef ESP32
    #include "Esp32Timer.h"

    /**@ingroup  GrpHAL
    * \brief    Type definition of function that initializes.
    *  \return
    */
    #define Timer_initItTimer       Esp32Timer_initItTimer


    /**@ingroup  GrpHAL
    * \brief    Type definition of function that stop timer.
    *  \return
    */
    #define Timer_stopItTimer       Esp32Timer_stopItTimer

    /**@ingroup  GrpHAL
    * \brief    Type definition of function that reload timer.
    *  \return
    */
    #define Timer_reloadItTimer       Esp32Timer_reloadItTimer

#else
    #error "board/hardware is not defined"
#endif

#endif /* _TIMER_H_ */

#include "Wifi.h"
#include "System.h"
#include "Initialize.h"
#include "CpuDelay.h"
#include "Gpio.h"
#include "MotorHandling.h"

/******************************************************************************
 * \fn      void app_main(void)
 * \param   void
 * \return  void
 *******************************************************************************/
void app_main(void)
{
    Initialize_init();
    Debug_info("Free Heap at %d = %d\n", __LINE__, OsGetFreeHeapSize());
    System_init();
    Debug_info("Free Heap at %d = %d\n", __LINE__, OsGetFreeHeapSize());
}

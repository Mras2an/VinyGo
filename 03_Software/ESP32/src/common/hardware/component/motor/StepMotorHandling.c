//#include <stdio.h>
//#include "CpuDelay.h"
//#include "Gpio.h"
//#include "StepMotorHandling.h"
//#include "Board.h"
//
////#define MOTOR_OFFSET 200 //360°
//#define MOTOR_OFFSET 495   //10cm
//#define MOTOR_LIMIT 800
//
///******************************************************************************
// * \fn                          void StepMotorHandling_init(void)
// * \param none
// * \return        none
// *******************************************************************************/
//void StepMotorHandling_init(void)
//{
//    //x
//    VR_ERROR_CHECK(
//        Gpio_configure(MOTOR_AXE_X_DIR, VR_GPIO_MODE_INPUT_OUTPUT, VR_GPIO_PULL_NONE));
//    VR_ERROR_CHECK(
//        Gpio_configure(MOTOR_AXE_X_CLK, VR_GPIO_MODE_INPUT_OUTPUT, VR_GPIO_PULL_NONE));
//    //    //y
//    //    VR_ERROR_CHECK(
//    //        Gpio_configure(MOTOR_AXE_Y_DIR, VR_GPIO_MODE_INPUT_OUTPUT, VR_GPIO_PULL_NONE));
//    //    VR_ERROR_CHECK(
//    //        Gpio_configure(MOTOR_AXE_Y_CLK, VR_GPIO_MODE_INPUT_OUTPUT, VR_GPIO_PULL_NONE));
//    //    // pump
//    //    VR_ERROR_CHECK(
//    //        Gpio_configure(MOTOR_PUMP_1, VR_GPIO_MODE_INPUT_OUTPUT, VR_GPIO_PULL_NONE));
//    //    VR_ERROR_CHECK(
//    //        Gpio_configure(MOTOR_PUMP_2, VR_GPIO_MODE_INPUT_OUTPUT, VR_GPIO_PULL_NONE));
//    //    VR_ERROR_CHECK(
//    //        Gpio_configure(MOTOR_PUMP_3, VR_GPIO_MODE_INPUT_OUTPUT, VR_GPIO_PULL_NONE));
//}
//
///******************************************************************************
// * \fn                          void StepMotorHandling_setPositionOnX(int position)
// * \param int
// * \return        none
// *******************************************************************************/
//void StepMotorHandling_setPositionOnX(int position)
//{
//    if(position == 0) {
//        StepMotorHandling_setInitialPosition();
//        return;
//    }
//
//    int end = 0;
//
//    if(position > 0) {
//        VR_ERROR_CHECK(Gpio_set(MOTOR_AXE_X_DIR, VR_LEVEL_LOW));
//        end = position * MOTOR_OFFSET;
//    } else {
//        VR_ERROR_CHECK(Gpio_set(MOTOR_AXE_X_DIR, VR_LEVEL_HIGH));
//        end = (position * (-1)) * MOTOR_OFFSET;
//    }
//
//    //  uint32_t bp1[1];
//    for(int i = 0; i < end; i++) {
//        VR_ERROR_CHECK(Gpio_set(MOTOR_AXE_X_CLK, VR_LEVEL_LOW));
//        CpuDelay_ms(1);
//        VR_ERROR_CHECK(Gpio_set(MOTOR_AXE_X_CLK, VR_LEVEL_HIGH));
//        CpuDelay_ms(1);
//        //    if (position < 0)
//        //    {
//        //      Gpio_get(DETECTION_AXE_X, bp1);
//        //      if (!bp1[0]) {
//        //          BarDebug_info("Stop motor.\n");
//        //        break;
//        //      }
//        //    }
//    }
//}
//
/////******************************************************************************
//// * \fn                          static void StepMotorHandling_get10ml()
//// * \param none
//// * \return        none
//// *******************************************************************************/
////static void StepMotorHandling_get10ml()
////{
//////    int end = MOTOR_LIMIT;
//////    VR_ERROR_CHECK(Gpio_set(MOTOR_AXE_Y_DIR, VR_LEVEL_LOW));
//////
//////    for(int i = 0; i < end; i++) {
//////        VR_ERROR_CHECK(Gpio_set(MOTOR_AXE_Y_CLK, VR_LEVEL_LOW));
//////        CpuDelay_ms(1);
//////        VR_ERROR_CHECK(Gpio_set(MOTOR_AXE_Y_CLK, VR_LEVEL_HIGH));
//////        CpuDelay_ms(1);
//////    }
//////
//////    CpuDelay_ms(600);
//////    uint32_t bp2[1];
//////    Gpio_get(DETECTION_AXE_Y, bp2);
//////    VR_ERROR_CHECK(Gpio_set(MOTOR_AXE_Y_DIR, VR_LEVEL_HIGH));
//////    end = MOTOR_LIMIT;
//////
//////    while(bp2[0] && (end != 0)) {
//////        Gpio_get(DETECTION_AXE_Y, bp2);
//////        VR_ERROR_CHECK(Gpio_set(MOTOR_AXE_Y_CLK, VR_LEVEL_LOW));
//////        CpuDelay_ms(1);
//////        VR_ERROR_CHECK(Gpio_set(MOTOR_AXE_Y_CLK, VR_LEVEL_HIGH));
//////        CpuDelay_ms(1);
//////        end--;
//////    }
//////
//////    CpuDelay_ms(800);
////}
////
/////******************************************************************************
//// * \fn                          static void StepMotorHandling_get25ml()
//// * \param void
//// * \return        none
//// *******************************************************************************/
////static void StepMotorHandling_get25ml()
////{
////    int end = MOTOR_LIMIT;
////    VR_ERROR_CHECK(Gpio_set(MOTOR_AXE_Y_DIR, VR_LEVEL_LOW));
////
////    for(int i = 0; i < end; i++) {
////        VR_ERROR_CHECK(Gpio_set(MOTOR_AXE_Y_CLK, VR_LEVEL_LOW));
////        CpuDelay_ms(1);
////        VR_ERROR_CHECK(Gpio_set(MOTOR_AXE_Y_CLK, VR_LEVEL_HIGH));
////        CpuDelay_ms(1);
////    }
////
////    CpuDelay_ms(2000);
////    uint32_t bp2[1];
////    Gpio_get(DETECTION_AXE_Y, bp2);
////    VR_ERROR_CHECK(Gpio_set(MOTOR_AXE_Y_DIR, VR_LEVEL_HIGH));
////    end = MOTOR_LIMIT;
////
////    while(bp2[0] && (end != 0)) {
////        Gpio_get(DETECTION_AXE_Y, bp2);
////        VR_ERROR_CHECK(Gpio_set(MOTOR_AXE_Y_CLK, VR_LEVEL_LOW));
////        CpuDelay_ms(1);
////        VR_ERROR_CHECK(Gpio_set(MOTOR_AXE_Y_CLK, VR_LEVEL_HIGH));
////        CpuDelay_ms(1);
////        end--;
////    }
////
////    CpuDelay_ms(800);
////}
////
/////******************************************************************************
//// * \fn                          void StepMotorHandling_getAMeasureOnY(int measure)
//// * \param int
//// * \return        none
//// *******************************************************************************/
////void StepMotorHandling_getAMeasureOnY(int measure)
////{
////    BarDebug_info("\t\t StepMotorHandling_getAMeasureOnY \n");
////
////    if(measure == 1) {
////        StepMotorHandling_get10ml();
////    }
////
////    if(measure == 2) {
////        StepMotorHandling_get25ml();
////    }
////
////    if(measure == 3) {
////        StepMotorHandling_get25ml();
////        StepMotorHandling_get10ml();
////    }
////
////    if(measure == 4) {
////        StepMotorHandling_get25ml();
////        StepMotorHandling_get25ml();
////    }
////
////    if(measure == 5) {
////        StepMotorHandling_get25ml();
////        StepMotorHandling_get25ml();
////        StepMotorHandling_get10ml();
////    }
////}
////
/////******************************************************************************
//// * \fn                          void StepMotorHandling_getAMeasureOnPump(int measure, int pump)
//// * \param int
//// * \param int
//// * \return        none
//// *******************************************************************************/
////void StepMotorHandling_getAMeasureOnPump(int measure, int pump)
////{
////    BarDebug_info("\t\t StepMotorHandling_getAMeasureOnPump \n");
////    VR_ERROR_CHECK(Gpio_set(pump, VR_LEVEL_HIGH));
////    CpuDelay_ms(500);
////
////    for(int j = 0; j < measure; j++) {
////        CpuDelay_ms(550);
////    }
////
////    VR_ERROR_CHECK(Gpio_set(pump, VR_LEVEL_LOW));
////}
////
/////******************************************************************************
//// * \fn                          void StepMotorHandling_setInitialPosition()
//// * \param none
//// * \return        none
//// *******************************************************************************/
////void StepMotorHandling_setInitialPosition()
////{
////    uint32_t bp1[1], bp2[1];
////    BarDebug_info("Set motor at the initial position on y...\n");
////    Gpio_get(DETECTION_AXE_Y, bp2);
////    VR_ERROR_CHECK(Gpio_set(MOTOR_AXE_Y_DIR, VR_LEVEL_HIGH));
////    int end = MOTOR_LIMIT;
////
////    while(bp2[0] && (end != 0)) {
////        Gpio_get(DETECTION_AXE_Y, bp2);
////        VR_ERROR_CHECK(Gpio_set(MOTOR_AXE_Y_CLK, VR_LEVEL_LOW));
////        CpuDelay_ms(1);
////        VR_ERROR_CHECK(Gpio_set(MOTOR_AXE_Y_CLK, VR_LEVEL_HIGH));
////        CpuDelay_ms(1);
////        end--;
////    }
////
////    BarDebug_info("Motor is now at the initial position on y.\n");
////    BarDebug_info("Set motor at the initial position on x...\n");
////    Gpio_get(DETECTION_AXE_X, bp1);
////    VR_ERROR_CHECK(Gpio_set(MOTOR_AXE_X_DIR, VR_LEVEL_HIGH));
////
////    while(bp1[0]) {
////        Gpio_get(DETECTION_AXE_X, bp1);
////        VR_ERROR_CHECK(Gpio_set(MOTOR_AXE_X_CLK, VR_LEVEL_LOW));
////        CpuDelay_ms(1);
////        VR_ERROR_CHECK(Gpio_set(MOTOR_AXE_X_CLK, VR_LEVEL_HIGH));
////        CpuDelay_ms(1);
////    }
////
////    BarDebug_info("Motor is now at the initial position on x.\n");
////}

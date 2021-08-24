#ifndef _BOARD_H_
#define _BOARD_H_

#include "Gpio.h"

#ifdef ESP32
    /*  INPUT: Addon*/
    #define ADD_ON                 VR_GPIO_NUM_2

    /*  OUTPUT/INPUT: for MIDI */
    #define TX_MIDI_3V3            VR_GPIO_NUM_4
    #define RX_MIDI_3V3            VR_GPIO_NUM_5

    /*  OUTPUT/INPUT: for LCD */
    #define TX_LCD_3V3             VR_GPIO_NUM_17
    #define RX_LCD_3V3             VR_GPIO_NUM_16

    /*  OUTPUT/INPUT: for Heater */
    #define CMD_HEATING_12V        VR_GPIO_NUM_27
    #define MCP_RES_CLK            VR_GPIO_NUM_19
    #define MCP_RES_CS             VR_GPIO_NUM_22
    #define MCP_RES_SDO            VR_GPIO_NUM_23
    #define MCP_RES_SDI            VR_GPIO_NUM_25
    #define PIN_NUM_MISO MCP_RES_SDO //SDO
    #define PIN_NUM_MOSI MCP_RES_SDI //SDI
    #define PIN_NUM_CLK  MCP_RES_CLK //clk

    /*  OUTPUT/INPUT: for VACUUM */
    #define VACUUM_CMD_BP1         VR_GPIO_NUM_32
    #define VACUUM_CMD_BP2         VR_GPIO_NUM_33

    /*  OUTPUT: cmd for CC motor*/
    #define CMD_MOTOR_EN1          VR_GPIO_NUM_15
    #define CMD_MOTOR_INA1         VR_GPIO_NUM_14

    /* INPUT: sensor, and switch */
    #define SENSOR_TRM             VR_GPIO_NUM_18
    #define HEAD_DETECT            VR_GPIO_NUM_34
    #define HALL_DETECT_7P         VR_GPIO_NUM_36
    #define HALL_DETECT_12P        VR_GPIO_NUM_39
    #define TRAY_DETECTION         VR_GPIO_NUM_35

    /* OUTPUT: cmd for step motor */
    #define STEP_MOTOR_DIR         VR_GPIO_NUM_12
    #define STEP_MOTOR_STEP        VR_GPIO_NUM_13
    #define STEP_MOTOR_ENABLE      VR_GPIO_NUM_21

    /* OUTPUT: Servo motor cmd */
    #define CMD_SERVO              VR_GPIO_NUM_26
#else
    #error "board/hardware is not defined"
#endif


#endif /* _BOARD_H_ */

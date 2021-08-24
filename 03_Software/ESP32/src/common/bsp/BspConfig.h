#ifndef _BSP_CONFIG_H_
#define _BSP_CONFIG_H_

#ifdef ESP32_4MB

    #define NB_BINARY_BLOCK           254
    #define HW_BLOCK_SIZE             4096

    /* Partition addr */
    #define ADDR_PART_1                0x00010000
    #define ADDR_PART_2                0x00110000
    #define ADDR_PART_3                0x00210000

#else
    #error "BSP config is not defined"
#endif

#endif // _BSP_CONFIG_H_

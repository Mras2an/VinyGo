#ifndef FILE_SYSTEM_H_
#define FILE_SYSTEM_H_

#ifdef ESP32
    #include "Esp32Fs.h"
    #define Fs_init       Esp32Fs_init
    #define Fs_write      Esp32Fs_write
    #define Fs_read       Esp32Fs_read
    #define Fs_delete     Esp32Fs_delete
    #define Fs_delete_all Esp32Fs_delete_all
    #define Fs_exists     Esp32Fs_exists
    #define Fs_write_i16  Esp32Fs_write_i16
    #define Fs_read_i16   Esp32Fs_read_i16
#else
    #error "board/hardware is not defined"
#endif

#endif /* FILE_SYSTEM_H_ */



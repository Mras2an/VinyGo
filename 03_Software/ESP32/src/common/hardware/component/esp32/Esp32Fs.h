#ifndef ESPFLASH_H_
#define ESPFLASH_H_

#include "nvs.h"

void Esp32Fs_init();

void Esp32Fs_cleanup();

int Esp32Fs_delete(const char * filename, const char * key);

int Esp32Fs_delete_all(const char * filename);

char * Esp32Fs_read(const char * filename, const char * key);

int16_t Esp32Fs_read_i16(const char * filename, const char * key);

esp_err_t Esp32Fs_write(const char * filename, const char * data, const char * key);

esp_err_t Esp32Fs_write_i16(const char * filename, const int16_t data, const char * key);

int Esp32Fs_getLength(const char * filename, const char * key);

int Esp32Fs_exists(const char * filename);

#endif /* ESPFLASH_H_ */

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "Worm.h"
#include "System.h"
#include "FileSystem.h"
#include "Bsp.h"
#include "BspConfig.h"
#include "SpeedVinyl.h"

typedef struct Worm_t {
    int value;
} Worm;

Worm Worm_t;
#define this (&Worm_t)

void Worm_init()
{
    this->value = 0;
    char * wrom = Fs_read(FILE_NAME_WROM, FILE_NAME_WROM);

    if(wrom != NULL)  {
        OsFree(wrom);
        this->value = 2;
        Debug_info("Worm config on 2mm\n");
    } else {
        this->value = 8;
        Debug_info("Worm config on 8mm\n");
    }
}

void Worm_convertFreqToTimeInString(int freq, char * buffer)
{
    //TODO check 8 or 2 and check 7p or 12p
    Debug_info("freq: %d\n", freq);
    float time = (float)(((1.0 / (float)freq) / 0.000028) / 60.0);

    // check if is a 7 or 12p
    if(SpeedVinyl_get() == 7) {
        time /= 3.0;
    } else if(SpeedVinyl_get() == 10) {
        time /= 1.5;
    } else {
        //useless time is already in 12p
    }

    //    if(SpeedVinyl_get33or45() == 33) {
    //        //useless time is already in 33TRM
    //    } else if(SpeedVinyl_get33or45() == 45) {
    //        time /= 1.36;
    //    }

    if(this->value == 8) {
        //useless worm is already in 8mm
    } else if(this->value == 2) {
        time *= 4;
    } else {
        Debug_err("No worm\n");
        return;
    }

    char tmp[10], *convert;
    memset(tmp, '\0', 10);
    sprintf(tmp, "%.2f", time);
    convert = strstr(tmp, ".");

    if(convert) {
        tmp[strlen(tmp) - strlen(convert)] = '\0';
        sprintf(buffer, "%sm%02ds", tmp, ((60 * atoi(&convert[1])) / 100));
    }
}

void Worm_toggleValue()
{
    char * wrom = Fs_read(FILE_NAME_WROM, FILE_NAME_WROM);

    if(wrom != NULL)  {
        OsFree(wrom);
        this->value = 8;
        Debug_info("Worm config on 8mm\n");
        Fs_delete_all(FILE_NAME_WROM);
    } else {
        this->value = 2;
        Debug_info("Worm config on 2mm\n");
        Fs_write(FILE_NAME_WROM, "true", FILE_NAME_WROM);
    }
}



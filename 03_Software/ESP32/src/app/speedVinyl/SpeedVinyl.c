#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "Debug.h"

typedef struct SpeedVinyl_t {
    int value;
} SpeedVinyl;

SpeedVinyl SpeedVinyl_t;
#define this (&SpeedVinyl_t)

void SpeedVinyl_init()
{
    this->value = 12;
    Debug_info("Vinyl config on 33TRM (12p)\n");
}

int SpeedVinyl_get()
{
    return this->value;
}

void SpeedVinyl_getString(char * buffer)
{
    if(this->value == 7) {
        memcpy(buffer, "7p", strlen("7p"));
        Debug_info("Vinyl config on 7p\n");
    } else if(this->value == 10) {
        memcpy(buffer, "10p", strlen("10p"));
        Debug_info("Vinyl config on 10p\n");
    } else {
        memcpy(buffer, "12p", strlen("12p)"));
        Debug_info("Vinyl config on 12p\n");
    }
}

void SpeedVinyl_increase()
{
    if(this->value == 7)
        this->value = 10;
    else if(this->value == 10)
        this->value = 12;
    else if(this->value == 12)
        this->value = 7;
}

void SpeedVinyl_decrease()
{
    if(this->value == 7)
        this->value = 12;
    else if(this->value == 10)
        this->value = 7;
    else if(this->value == 12)
        this->value = 10;
}

void SpeedVinyl_set(int val)
{
    if(val == 7 || val == 10 || val == 12)
        this->value = val;
    else
        this->value = 12;
}



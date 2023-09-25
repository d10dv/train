#ifndef __CONVERSION_UTILS_H__
#define __CONVERSION_UTILS_H__

#include "main.h"

typedef struct
{
    uint16_t low_in;
    uint16_t hight_in;
    uint16_t low_out;
    uint16_t hight_out;
    uint16_t input;
} conversion_t;

int32_t conversion(conversion_t *conversion);

#endif /* __CONVERSION_UTILS_H__ */
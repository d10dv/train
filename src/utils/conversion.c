#include "conversion.h"

int32_t conversion(conversion_t *conversion)
{
    return (int32_t)conversion->input * (conversion->hight_out - conversion->low_out) / (conversion->hight_in - conversion->low_in) + conversion->low_out;
}
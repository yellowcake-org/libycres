#ifndef LIB_YCRES_MATH_METHODS_H
#define LIB_YCRES_MATH_METHODS_H

#include <stdint.h>

typedef struct yc_res_math_location {
    uint16_t grid_idx;

    uint8_t elevation_idx;
    uint8_t orientation_idx;
} yc_res_math_location_t;

#endif //LIB_YCRES_MATH_METHODS_H

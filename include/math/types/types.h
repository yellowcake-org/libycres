#ifndef INCLUDE_MATH_TYPES_H
#define INCLUDE_MATH_TYPES_H

#include <stdint.h>

typedef enum yc_res_math_orientation {
    YC_RES_MATH_ORIENTATION_NE,
    YC_RES_MATH_ORIENTATION_E,
    YC_RES_MATH_ORIENTATION_SE,
    YC_RES_MATH_ORIENTATION_SW,
    YC_RES_MATH_ORIENTATION_W,
    YC_RES_MATH_ORIENTATION_NW,
    YC_RES_MATH_ORIENTATION_COUNT
} yc_res_math_orientation_t;

typedef struct yc_res_math_location {
    uint16_t grid_idx;

    uint8_t elevation_idx;
    uint8_t orientation_idx;
} yc_res_math_location_t;

typedef enum yc_res_math_grid_size {
    YC_RES_MATH_GRID_SIZE_TILES = 100,
    YC_RES_MATH_GRID_SIZE_HEXES = 200
} yc_res_math_grid_size_t;

#endif //INCLUDE_MATH_TYPES_H

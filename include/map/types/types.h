#ifndef LIB_YCRES_MAP_TYPES_H
#define LIB_YCRES_MAP_TYPES_H

#include "level/level.h"
#include "elevation/elevation.h"

typedef struct yc_res_map {
    bool is_save;
    uint32_t map_idx, script_id;

    uint32_t ticks;
    yc_res_pro_destination_t entrance;

    size_t count_local_variables;
    int32_t *local_variables;

    size_t count_global_variables;
    int32_t *global_variables;

    yc_res_map_level_t *levels[YC_RES_MAP_ELEVATION_COUNT];
} yc_res_map_t;

#endif //LIB_YCRES_MAP_TYPES_H

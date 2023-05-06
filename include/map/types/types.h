#ifndef LIB_YCRES_MAP_TYPES_H
#define LIB_YCRES_MAP_TYPES_H

#include "elevation/elevation.h"
#include "script/script.h"
#include "level/level.h"

typedef struct yc_res_map {
    bool is_save;
    uint32_t map_idx, script_id;

    uint32_t ticks;
    yc_res_pro_destination_t entrance;

    size_t count_lvars;
    int32_t *lvars;

    size_t count_gvars;
    int32_t *gvars;

    size_t count_scripts;
    yc_res_map_script_t *scripts;

    yc_res_map_level_t *levels[YC_RES_MAP_ELEVATION_COUNT];
} yc_res_map_t;

#endif //LIB_YCRES_MAP_TYPES_H

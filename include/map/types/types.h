#ifndef LIB_YCRES_MAP_TYPES_H
#define LIB_YCRES_MAP_TYPES_H

#include "elevation/elevation.h"
#include "variables/variables.h"
#include "script/script.h"
#include "level/level.h"

typedef struct yc_res_map {
    bool is_save;
    uint32_t map_idx, script_idx; // script is SYSTEM script

    uint32_t ticks;
    yc_res_math_location_t entrance;

    yc_res_map_scripts_t scripts;
    yc_res_map_variables_t local, global;

    yc_res_map_level_t *levels[YC_RES_MAP_ELEVATION_COUNT];
} yc_res_map_t;

#endif //LIB_YCRES_MAP_TYPES_H

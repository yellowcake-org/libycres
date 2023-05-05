#ifndef LIB_YCRES_MAP_TYPES_LEVEL_H
#define LIB_YCRES_MAP_TYPES_LEVEL_H

#include "tiles/tiles.h"

typedef struct yc_res_map_level {
    yc_res_map_level_tiles_t floor, roof;
} yc_res_map_level_t;

#endif //LIB_YCRES_MAP_TYPES_LEVEL_H
